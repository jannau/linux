// SPDX-License-Identifier: GPL-2.0-only

#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/dma-mapping.h>
#include <linux/of.h>
#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_device.h>
#include <drm/drm_drv.h>
#include <drm/drm_edid.h>
#include <drm/drm_fb_dma_helper.h>
#include <drm/drm_fbdev_generic.h>
#include <drm/drm_framebuffer.h>
#include <drm/drm_gem_dma_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_simple_kms_helper.h>
#include <drm/drm_vblank.h>
#include <drm/drm_gem_atomic_helper.h>
#include <drm/drm_damage_helper.h>

#define ADP_INT_STATUS 0x34
#define ADP_INT_STATUS_INT_MASK 0x7
#define ADP_INT_STATUS_VBLANK 0x1
#define ADP_CTRL 0x100
#define ADP_CTRL_VBLANK_ON 0x12
#define ADP_CTRL_FIFO_ON 0x601
#define ADP_SCREEN_SIZE 0x0c

#define ADBE_FIFO 0x10c0
#define ADBE_FIFO_SYNC 0xc0000000

#define ADBE_BLEND_BYPASS 0x2020
#define ADBE_BLEND_EN1 0x2028
#define ADBE_BLEND_EN2 0x2074
#define ADBE_BLEND_EN3 0x202c
#define ADBE_BLEND_EN4 0x2034
#define ADBE_MASK_BUF 0x2200

#define ADBE_SRC_START 0x4040
#define ADBE_SRC_SIZE 0x4048
#define ADBE_DST_START 0x4050
#define ADBE_DST_SIZE 0x4054
#define ADBE_STRIDE 0x4038
#define ADBE_FB_BASE 0x4030

#define ADBE_LAYER_EN1 0x4020
#define ADBE_LAYER_EN2 0x4068
#define ADBE_LAYER_EN3 0x40b4
#define ADBE_LAYER_EN4 0x40f4
#define ADBE_SCALE_CTL 0x40ac
#define ADBE_SCALE_CTL_BYPASS 0x100000

#define ADBE_LAYER_CTL 0x1038
#define ADBE_LAYER_CTL_ENABLE 0x10000

#define ADBE_PIX_FMT 0x402c
#define ADBE_PIX_FMT_XRGB32 0x53e4001

DEFINE_DRM_GEM_DMA_FOPS(adp_fops);

static int adp_drm_gem_dumb_create(struct drm_file *file_priv,
					struct drm_device *drm,
					struct drm_mode_create_dumb *args)
{
	args->pitch = ALIGN(DIV_ROUND_UP(args->width * args->bpp, 8), 64);
	args->size = args->pitch * args->height;

	return drm_gem_dma_dumb_create_internal(file_priv, drm, args);
}

static const struct drm_driver adp_driver = {
	.driver_features = DRIVER_GEM | DRIVER_MODESET | DRIVER_ATOMIC,
	.fops = &adp_fops,
	DRM_GEM_DMA_DRIVER_OPS_VMAP_WITH_DUMB_CREATE(adp_drm_gem_dumb_create),
	.name = "adp",
	.desc = "Apple Display Pipe DRM Driver",
	.date = "20230412",
	.major = 0,
	.minor = 1,
};

struct adp_drv_private {
	struct drm_device drm;
	struct drm_crtc crtc;
	struct drm_encoder encoder;
	struct drm_connector connector;
	void __iomem *be;
	void __iomem *fe;
	u32 *mask_buf;
	u64 mask_buf_size;
	dma_addr_t mask_iova;
	int be_irq;
	int fe_irq;
	spinlock_t irq_lock;
	struct drm_pending_vblank_event *event;
};

struct adp_plane {
	struct drm_plane base_plane;
	u8 id;
};

struct adp_framebuffer {
	struct drm_framebuffer base;
	u32 *shadow_buf;
	u64 shadow_size;
	dma_addr_t shadow_iova;
	u32 stride;
	u32 height;
};

#define to_adp_plane(x) container_of(x, struct adp_plane, base_plane)
#define to_adp(x) container_of(x, struct adp_drv_private, drm)
#define crtc_to_adp(x) container_of(x, struct adp_drv_private, crtc)
#define to_adp_fb(x) container_of(x, struct adp_framebuffer, base)
#define conn_to_adp(x) container_of(x, struct adp_drv_private, connector)

static int adp_plane_atomic_check(struct drm_plane *plane,
				    struct drm_atomic_state *state)
{
	struct drm_plane_state *new_plane_state;
	struct drm_crtc_state *crtc_state;

	new_plane_state = drm_atomic_get_new_plane_state(state, plane);

	if (!new_plane_state->crtc)
		return 0;

	crtc_state = drm_atomic_get_crtc_state(state, new_plane_state->crtc);
	if (IS_ERR(crtc_state))
		return PTR_ERR(crtc_state);
	return drm_atomic_helper_check_plane_state(new_plane_state,
						   crtc_state,
						   DRM_PLANE_NO_SCALING,
						   DRM_PLANE_NO_SCALING,
						   true, true);
}

static void adp_handle_damage(struct adp_framebuffer *afb, struct drm_framebuffer *fb,
			      struct drm_rect *damage, struct iosys_map *map) {
	int x, y;
	u64 src_pos, dst_pos;
	u32 *vaddr = map->vaddr;

	for (x = damage->x1; x < damage->x2; x++) {
		for (y = damage->y1; y < damage->y2; y++) {
			src_pos = y * (fb->pitches[0] / 4) + x;
			dst_pos = (afb->height - y) + x * afb->stride;
			afb->shadow_buf[dst_pos] = vaddr[src_pos];
		}
	}
}

static void adp_plane_atomic_update(struct drm_plane *plane,
				    struct drm_atomic_state *state)
{
	struct adp_drv_private *adp;
	struct adp_plane *adp_plane;
	struct drm_rect src_rect;
	struct drm_gem_dma_object *obj;
	struct drm_framebuffer *fb;
	struct adp_framebuffer *afb;
	struct drm_plane_state *new_state = drm_atomic_get_new_plane_state(state, plane);
	struct drm_plane_state *old_state = drm_atomic_get_old_plane_state(state, plane);
	struct drm_shadow_plane_state *shadow_plane_state = to_drm_shadow_plane_state(new_state);
	struct drm_atomic_helper_damage_iter iter;
	struct drm_rect damage;
	u32 src_pos, src_size, dst_pos, dst_size;
	if (!plane || !new_state || !old_state)
		return;

	fb = new_state->fb;
	if (!fb)
		return;
	adp_plane = to_adp_plane(plane);
	adp = to_adp(plane->dev);
	afb = to_adp_fb(fb);

	drm_gem_fb_begin_cpu_access(fb, DMA_FROM_DEVICE);

	drm_atomic_helper_damage_iter_init(&iter, old_state, new_state);
	drm_atomic_for_each_plane_damage(&iter, &damage) {
		adp_handle_damage(afb, fb, &damage, &shadow_plane_state->map[0]);
	}

	drm_gem_fb_end_cpu_access(fb, DMA_FROM_DEVICE);

	drm_rect_fp_to_int(&src_rect, &new_state->src);
	src_pos = src_rect.y1 << 16 | src_rect.x1;
	dst_pos = new_state->dst.y1 << 16 | new_state->dst.x1;
	src_size = drm_rect_height(&src_rect) << 16 | drm_rect_width(&src_rect);
	dst_size = drm_rect_height(&new_state->dst) << 16 |
		drm_rect_width(&new_state->dst);
	writel(src_pos, adp->be + ADBE_SRC_START);
	writel(src_size, adp->be + ADBE_SRC_SIZE);
	writel(dst_pos, adp->be + ADBE_DST_START);
	writel(dst_size, adp->be + ADBE_DST_SIZE);
	writel(afb->stride * 4, adp->be + ADBE_STRIDE);
	obj = drm_fb_dma_get_gem_obj(fb, 0);
	if (obj)
		writel(afb->shadow_iova, adp->be + ADBE_FB_BASE);

	writel(0x1, adp->be + ADBE_LAYER_EN1);
	writel(0x1, adp->be + ADBE_LAYER_EN2);
	writel(0x1, adp->be + ADBE_LAYER_EN3);
	writel(0x1, adp->be + ADBE_LAYER_EN4);
	writel(ADBE_SCALE_CTL_BYPASS, adp->be + ADBE_SCALE_CTL);
	writel(ADBE_LAYER_CTL_ENABLE | 0x1, adp->be + ADBE_LAYER_CTL);
	writel(ADBE_PIX_FMT_XRGB32, adp->be + ADBE_PIX_FMT);

}

static void adp_plane_atomic_disable(struct drm_plane *plane,
				     struct drm_atomic_state *state)
{
	struct adp_drv_private *adp = to_adp(plane->dev);
	writel(0x0, adp->be + ADBE_LAYER_EN1);
	writel(0x0, adp->be + ADBE_LAYER_EN2);
	writel(0x0, adp->be + ADBE_LAYER_EN3);
	writel(0x0, adp->be + ADBE_LAYER_EN4);
	writel(ADBE_LAYER_CTL_ENABLE, adp->be + ADBE_LAYER_CTL);
}

static const struct drm_plane_helper_funcs adp_plane_helper_funcs = {
	.atomic_check = adp_plane_atomic_check,
	.atomic_update = adp_plane_atomic_update,
	.atomic_disable = adp_plane_atomic_disable,
	DRM_GEM_SHADOW_PLANE_HELPER_FUNCS
};

static const struct drm_plane_funcs adp_plane_funcs = {
	.update_plane = drm_atomic_helper_update_plane,
	.disable_plane = drm_atomic_helper_disable_plane,
	DRM_GEM_SHADOW_PLANE_FUNCS
};

static const u32 plane_formats[] = {
	DRM_FORMAT_XRGB8888,
};

#define ALL_CRTCS 1

struct adp_plane *adp_plane_new(struct adp_drv_private *adp, u8 id)
{
	struct drm_device *drm = &adp->drm;
	struct adp_plane *plane;
	enum drm_plane_type plane_type;

	plane_type = (id == 0) ? DRM_PLANE_TYPE_PRIMARY :
		DRM_PLANE_TYPE_OVERLAY;

	plane = drmm_universal_plane_alloc(drm, struct adp_plane, base_plane,
					   ALL_CRTCS, &adp_plane_funcs,
					   plane_formats, ARRAY_SIZE(plane_formats),
					   NULL, plane_type, "plane %d", id);
	if (!plane) {
		drm_err(drm, "failed to allocate plane");
		return ERR_PTR(-ENOMEM);
	}
	plane->id = id;

	drm_plane_helper_add(&plane->base_plane, &adp_plane_helper_funcs);
	drm_plane_enable_fb_damage_clips(&plane->base_plane);
	return plane;
}

static void adp_enable_vblank(struct adp_drv_private *adp)
{
	u32 cur_ctrl;

	writel(ADP_INT_STATUS_INT_MASK, adp->fe + ADP_INT_STATUS);

	cur_ctrl = readl(adp->fe + ADP_CTRL);
	writel(cur_ctrl | ADP_CTRL_VBLANK_ON, adp->fe + ADP_CTRL);
}

static int adp_crtc_enable_vblank(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct adp_drv_private *adp = to_adp(dev);
	adp_enable_vblank(adp);

	return 0;
}

static void adp_disable_vblank(struct adp_drv_private *adp)
{
	u32 cur_ctrl;

	cur_ctrl = readl(adp->fe + ADP_CTRL);
	writel(cur_ctrl & ~ADP_CTRL_VBLANK_ON, adp->fe + ADP_CTRL);
	writel(ADP_INT_STATUS_INT_MASK, adp->fe + ADP_INT_STATUS);
}

static void adp_crtc_disable_vblank(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct adp_drv_private *adp = to_adp(dev);

	adp_disable_vblank(adp);
}


static void adp_crtc_atomic_enable(struct drm_crtc *crtc,
				   struct drm_atomic_state *state)
{
	struct adp_drv_private *adp = crtc_to_adp(crtc);
	writel(0x1, adp->be + ADBE_BLEND_EN2);
	writel(0x10, adp->be + ADBE_BLEND_EN1);
	writel(0x1, adp->be + ADBE_BLEND_EN3);
	writel(0x1, adp->be + ADBE_BLEND_BYPASS);
	writel(0x1, adp->be + ADBE_BLEND_EN4);
}

static void adp_crtc_atomic_disable(struct drm_crtc *crtc,
				    struct drm_atomic_state *state)
{
	struct adp_drv_private *adp = crtc_to_adp(crtc);
	struct drm_crtc_state *old_state = drm_atomic_get_old_crtc_state(state, crtc);

	drm_atomic_helper_disable_planes_on_crtc(old_state, false);

	writel(0x0, adp->be + ADBE_BLEND_EN2);
	writel(0x0, adp->be + ADBE_BLEND_EN1);
	writel(0x0, adp->be + ADBE_BLEND_EN3);
	writel(0x0, adp->be + ADBE_BLEND_BYPASS);
	writel(0x0, adp->be + ADBE_BLEND_EN4);
	drm_crtc_vblank_off(crtc);
}

static void adp_crtc_atomic_flush(struct drm_crtc *crtc,
				  struct drm_atomic_state *state)
{
	u32 frame_num = 1;
	struct adp_drv_private *adp = crtc_to_adp(crtc);
	struct drm_crtc_state *new_state = drm_atomic_get_new_crtc_state(state, crtc);
	u64 new_size = ALIGN(new_state->mode.hdisplay *
			     new_state->mode.vdisplay * 4, PAGE_SIZE);

	if (new_size != adp->mask_buf_size) {
		if (adp->mask_buf)
			dma_free_coherent(crtc->dev->dev, adp->mask_buf_size,
					  adp->mask_buf, adp->mask_iova);
		adp->mask_buf = NULL;
		if (new_size != 0) {
			adp->mask_buf = dma_alloc_coherent(crtc->dev->dev, new_size,
							   &adp->mask_iova, GFP_KERNEL);
			memset(adp->mask_buf, 0xFF, new_size);
			writel(adp->mask_iova, adp->be + ADBE_MASK_BUF);
		}
		adp->mask_buf_size = new_size;
	}
	//	printk(KERN_ERR "flushing, irq state %x", readl(adp->fe + ADP_CTRL));
	writel(ADBE_FIFO_SYNC | frame_num, adp->be + ADBE_FIFO);
	//FIXME: use adbe flush interrupt
	spin_lock_irq(&crtc->dev->event_lock);
	if (crtc->state->event) {
	        drm_crtc_vblank_get(crtc);
		adp->event = crtc->state->event;
	}
	crtc->state->event = NULL;
	spin_unlock_irq(&crtc->dev->event_lock);
}

static const struct drm_crtc_funcs adp_crtc_funcs = {
	.destroy = drm_crtc_cleanup,
	.set_config = drm_atomic_helper_set_config,
	.page_flip = drm_atomic_helper_page_flip,
	.reset = drm_atomic_helper_crtc_reset,
	.atomic_duplicate_state = drm_atomic_helper_crtc_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_crtc_destroy_state,
	.enable_vblank = adp_crtc_enable_vblank,
	.disable_vblank = adp_crtc_disable_vblank,
};


static const struct drm_crtc_helper_funcs adp_crtc_helper_funcs = {
	.atomic_enable = adp_crtc_atomic_enable,
	.atomic_disable = adp_crtc_atomic_disable,
	.atomic_flush = adp_crtc_atomic_flush,
};

static int adp_setup_crtc(struct adp_drv_private *adp)
{
	struct drm_device *drm = &adp->drm;
	struct adp_plane *primary;
	int ret;

	primary = adp_plane_new(adp, 0);
	if (IS_ERR(primary))
		return PTR_ERR(primary);

	ret = drm_crtc_init_with_planes(drm, &adp->crtc, &primary->base_plane,
					NULL, &adp_crtc_funcs, NULL);
	if (ret)
		return ret;

	drm_crtc_helper_add(&adp->crtc, &adp_crtc_helper_funcs);
	return 0;
}

static int adp_get_modes(struct drm_connector *connector)
{
	struct adp_drv_private *adp = conn_to_adp(connector);
	struct drm_display_mode *mode;
	u32 size;

	size = readl(adp->fe + ADP_SCREEN_SIZE);
	mode = drm_cvt_mode(connector->dev, size >> 16, size & 0xFFFF, 60, true, false, false);
	drm_mode_probed_add(connector, mode);
	return 1;
}

static const struct drm_connector_funcs adp_connector_funcs = {
	.fill_modes = drm_helper_probe_single_connector_modes,
	.destroy = drm_connector_cleanup,
	.reset = drm_atomic_helper_connector_reset,
	.atomic_duplicate_state	= drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_connector_destroy_state,
};

static const struct drm_connector_helper_funcs adp_connector_helper_funcs = {
	.get_modes = adp_get_modes
};

static void adp_fb_destroy(struct drm_framebuffer *fb) {
	struct adp_framebuffer *afb = to_adp_fb(fb);
	dma_free_coherent(fb->dev->dev, afb->shadow_size,
			  afb->shadow_buf, afb->shadow_iova);
	drm_gem_fb_destroy(fb);
}

static const struct drm_framebuffer_funcs adp_fb_funcs = {
	.destroy	= adp_fb_destroy,
	.create_handle	= drm_gem_fb_create_handle,
	.dirty		= drm_atomic_helper_dirtyfb,
};

struct drm_framebuffer *adp_fb_create(struct drm_device *dev, struct drm_file *file,
				      const struct drm_mode_fb_cmd2 *mode_cmd) {
	struct adp_framebuffer *fb;
	int ret;
	u64 size;

	fb = kzalloc(sizeof(*fb), GFP_KERNEL);
	if (!fb)
		return ERR_PTR(-ENOMEM);

	ret = drm_gem_fb_init_with_funcs(dev, &fb->base, file, mode_cmd, &adp_fb_funcs);
	if (ret) {
		kfree(fb);
		return ERR_PTR(ret);
	}
	fb->stride = ALIGN(mode_cmd->height, 64);
	size = ALIGN(mode_cmd->width * fb->stride * 4, PAGE_SIZE);
	fb->shadow_size = size;
	fb->shadow_buf = dma_alloc_coherent(dev->dev, size,
					    &fb->shadow_iova, GFP_KERNEL);
	fb->height = mode_cmd->height;

	return &fb->base;
}

static const struct drm_mode_config_funcs adp_mode_config_funcs = {
	.fb_create = adp_fb_create,
	.atomic_check = drm_atomic_helper_check,
	.atomic_commit = drm_atomic_helper_commit,
};

static int adp_setup_mode_config(struct adp_drv_private *adp)
{
	struct drm_device *drm = &adp->drm;
	int ret;

	ret = drmm_mode_config_init(drm);
	if (ret)
		return ret;

	drm->mode_config.min_width = 32;
	drm->mode_config.min_height = 32;
	drm->mode_config.max_width = 16384;
	drm->mode_config.max_height = 16384;
	drm->mode_config.preferred_depth = 24;
	drm->mode_config.prefer_shadow = 0;
	drm->mode_config.funcs = &adp_mode_config_funcs;

	ret = adp_setup_crtc(adp);
	if (ret) {
		drm_err(drm, "failed to create crtc");
		return ret;
	}

	adp->encoder.possible_crtcs = ALL_CRTCS;
	ret = drm_simple_encoder_init(drm, &adp->encoder, DRM_MODE_ENCODER_DSI);
	if (ret) {
		drm_err(drm, "failed to init encoder");
		return ret;
	}
	drm_connector_helper_add(&adp->connector,
				 &adp_connector_helper_funcs);
	ret = drm_connector_init(drm, &adp->connector, &adp_connector_funcs,
				 DRM_MODE_CONNECTOR_DSI);
	if (ret)
		return ret;

	drm_connector_attach_encoder(&adp->connector, &adp->encoder);

	ret = drm_vblank_init(drm, drm->mode_config.num_crtc);
	if (ret < 0) {
		drm_err(drm, "failed to initialize vblank");
		return ret;
	}

	drm_mode_config_reset(drm);

	return 0;
}

static int adp_parse_of(struct platform_device *pdev, struct adp_drv_private *adp)
{
	adp->be = devm_platform_ioremap_resource_byname(pdev, "be");
	if (IS_ERR(adp->be)) {
		dev_err(&pdev->dev, "failed to map display backend mmio");
		return PTR_ERR(adp->be);
	}

	adp->fe = devm_platform_ioremap_resource_byname(pdev, "fe");
	if (IS_ERR(adp->fe)) {
		dev_err(&pdev->dev, "failed to map display pipe mmio");
		return PTR_ERR(adp->fe);
	}

	adp->be_irq = platform_get_irq_byname(pdev, "be");
	if (adp->be_irq < 0) {
		dev_err(&pdev->dev, "failed to find be irq");
		return adp->be_irq;
	}

	adp->fe_irq = platform_get_irq_byname(pdev, "fe");
	if (adp->fe_irq < 0) {
		dev_err(&pdev->dev, "failed to find fe irq");
		return adp->fe_irq;
	}
	return 0;
}

static irqreturn_t adp_fe_irq(int irq, void *arg)
{
	struct adp_drv_private *adp = (struct adp_drv_private *)arg;
	u32 int_status;
	u32 int_ctl;

	spin_lock(&adp->irq_lock);

	int_status = readl(adp->fe + ADP_INT_STATUS);
	if (int_status & ADP_INT_STATUS_VBLANK) {
		drm_crtc_handle_vblank(&adp->crtc);
		spin_lock(&adp->crtc.dev->event_lock);
		if (adp->event) {
			int_ctl = readl(adp->fe + ADP_CTRL);
			if ((int_ctl & 0xF00) == 0x600) {
				drm_crtc_send_vblank_event(&adp->crtc, adp->event);
				adp->event = NULL;
				drm_crtc_vblank_put(&adp->crtc);
			}
		}
		spin_unlock(&adp->crtc.dev->event_lock);
	}

	writel(int_status, adp->fe + ADP_INT_STATUS);

	spin_unlock(&adp->irq_lock);

	return IRQ_HANDLED;
}

static int adp_probe(struct platform_device *pdev)
{
	struct adp_drv_private *adp;
	void __iomem *mipi;
	int err;

	adp = devm_drm_dev_alloc(&pdev->dev, &adp_driver, struct adp_drv_private, drm);
	if (IS_ERR(adp))
		return PTR_ERR(adp);

	spin_lock_init(&adp->irq_lock);

	dev_set_drvdata(&pdev->dev, &adp->drm);

	err = adp_parse_of(pdev, adp);
	if (err < 0)
		return err;

	mipi = devm_platform_ioremap_resource_byname(pdev, "mipi");
	if (IS_ERR(mipi))
		return PTR_ERR(mipi);

	writel(0x8051, mipi + 0x70);
	writel(0x229, mipi + 0x6c);
	adp_disable_vblank(adp);
	writel(ADP_CTRL_FIFO_ON | ADP_CTRL_VBLANK_ON, adp->fe + ADP_CTRL);

	err = adp_setup_mode_config(adp);
	if (err < 0)
		return err;

	err = devm_request_irq(&pdev->dev, adp->fe_irq, adp_fe_irq, 0,
			       "adp-fe", adp);
	if (err)
		return err;

	err = drm_dev_register(&adp->drm, 0);
	if (err)
		return err;
	return 0;
}

static int adp_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct drm_device *drm = dev_get_drvdata(dev);
	struct adp_drv_private *adp = to_adp(drm);

	adp_disable_vblank(adp);
	drm_dev_unregister(drm);
	dev_set_drvdata(dev, NULL);
	drm_atomic_helper_shutdown(drm);
	return 0;
}

static const struct of_device_id adp_of_match[] = {
	{ .compatible = "apple,h7-display-pipe", },
	{ },
};
MODULE_DEVICE_TABLE(of, adp_of_match);

static struct platform_driver adp_platform_driver = {
	.driver = {
		.name = "adp",
		.of_match_table = adp_of_match,
	},
	.probe = adp_probe,
	.remove = adp_remove,
};

module_platform_driver(adp_platform_driver);

MODULE_DESCRIPTION("Apple Display Pipe DRM driver");
MODULE_LICENSE("GPL v2");
