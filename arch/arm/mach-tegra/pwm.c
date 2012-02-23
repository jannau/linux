/*
 * linux/arch/arm/mach-pxa/pwm.c
 *
 * simple driver for PWM (Pulse Width Modulator) controller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * 2008-02-13	initial version
 * 		eric miao <eric.miao@marvell.com>
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/pwm.h>

#include <asm/div64.h>
#include <mach/iomap.h>
#include <mach/clk.h>
#include <linux/delay.h>

#include <linux/gpio.h> //for gpio test
#include <mach/gpio-names.h> //gor gpio test

struct pwm_device {
	struct list_head	node;
	struct platform_device	*pdev;

	const char	*label;
	struct clk	*clk;
	int		clk_enabled;
	void __iomem	*mmio_base;

	unsigned int	use_count;
	unsigned int	pwm_id;
};

int pwm_config(struct pwm_device *pwm, int duty_ns, int period_ns)
{
  	unsigned long long div_ll;
    unsigned long reg_bak, dc, cur_rate, div;
    unsigned long parent_cur_rate;
	struct clk *parent;
	if (pwm == NULL || period_ns == 0 || duty_ns > period_ns)
		return -EINVAL;
    //printk("jh.chun  pwm_config  duty_ns is %d\n", duty_ns);  
    //printk("jh.chun  pwm_config  period_ns is %d\n", period_ns);  	

   	parent=clk_get_parent(pwm->clk);
	parent_cur_rate = clk_get_rate (parent);
    //printk("jh.chun  pwm_config  parent_cur_rate is %d\n", parent_cur_rate);
	clk_set_rate(pwm->clk,17000000);
	cur_rate = clk_get_rate(pwm->clk);

    //printk("jh.chun  pwm_config  cur_rate is %d\n", cur_rate);   
    cur_rate /= 256;
	div_ll = (cur_rate/1000) * (period_ns/1000); //convert to khz and micro sec.
    //printk("jh.chun  before div pwm_config  div_ll is %lld\n", div_ll);  
	do_div(div_ll, 1000);
     //printk("jh.chun  after div pwm_config  div_ll is %lld\n", div_ll);  
    div = div_ll;

	if (div < 1)
		div = 1;

	if (div > ((1<<13)-1)){
        printk("jh.chun  pwm_config  div is large\n");   
		return -EINVAL;
	}
    //printk("jh.chun  pwm_config  div is %d\n", div);   

    dc = (256*duty_ns)/period_ns;
    //printk("jh.chun  pwm_config dc= %d \n", dc);

    pwm_enable(pwm);
    //pwm->pwm_id = 0;
    reg_bak = readl(pwm->mmio_base+pwm->pwm_id*0x10);
    //printk("jh.chun  read pwm reg : 0x%x\n",reg_bak);

    reg_bak = ((1<<31))|((div/20) & 0x1fff)|((dc & 0x3fff)<<16);
    //reg_bak = ((1<<31))|(1& 0x1fff)|((dc & 0x3fff)<<16); //for test
    //printk("jh.chun  reg_bak before write : 0x%x\n",reg_bak);
    gpio_free(TEGRA_GPIO_PU3);
    writel(reg_bak, (pwm->mmio_base + pwm->pwm_id * 0x10));
    //writel(reg_bak, (pwm->mmio_base + 0x10));
    //ch.han GPIO test
    //gpio_request(TEGRA_GPIO_PU3,"PWM_GPIO_TEST");
    //gpio_direction_output(TEGRA_GPIO_PU3,1);
    
    reg_bak = readl(pwm->mmio_base+pwm->pwm_id*0x10);

    //printk("jh.chun  reg_bak after write : 0x%x\n",reg_bak);

	return 0;
}
EXPORT_SYMBOL(pwm_config);

int pwm_enable(struct pwm_device *pwm)
{
	int rc = 0;

	if (!pwm->clk_enabled) {
		rc = clk_enable(pwm->clk);
		if (!rc)
			pwm->clk_enabled = 1;
	}
	return rc;
}
EXPORT_SYMBOL(pwm_enable);

void pwm_disable(struct pwm_device *pwm)
{
	if (pwm->clk_enabled) {
		clk_disable(pwm->clk);
		pwm->clk_enabled = 0;
	}
}
EXPORT_SYMBOL(pwm_disable);

static DEFINE_MUTEX(pwm_lock);
static LIST_HEAD(pwm_list);

struct pwm_device *pwm_request(int pwm_id, const char *label)
{
	struct pwm_device *pwm;
	int found = 0;

	mutex_lock(&pwm_lock);

	list_for_each_entry(pwm, &pwm_list, node) {
		if (pwm->pwm_id == pwm_id) {
			found = 1;
			break;
		}
	}


	if (found) {
		if (pwm->use_count == 0) {
			pwm->use_count++;
			pwm->label = label;
		} else
			pwm = ERR_PTR(-EBUSY);
	} else
	{
        pr_alert("PWM request error\n");
		pwm = ERR_PTR(-ENOENT);
	}
	mutex_unlock(&pwm_lock);
	return pwm;
}
EXPORT_SYMBOL(pwm_request);

void pwm_free(struct pwm_device *pwm)
{
	mutex_lock(&pwm_lock);

	if (pwm->use_count) {
		pwm->use_count--;
		pwm->label = NULL;
	} else
		pr_warning("PWM device already freed\n");

	mutex_unlock(&pwm_lock);
}
EXPORT_SYMBOL(pwm_free);

static inline void __add_pwm(struct pwm_device *pwm)
{
	mutex_lock(&pwm_lock);
	list_add_tail(&pwm->node, &pwm_list);
	mutex_unlock(&pwm_lock);
}

static int __devinit pwm_probe(struct platform_device *pdev)
{
//	struct platform_device_id *id = platform_get_device_id(pdev);
	struct pwm_device *pwm = NULL;
	struct resource *r;
	int ret = 0;


	pwm = kzalloc(sizeof(struct pwm_device), GFP_KERNEL);
	if (pwm == NULL) {
		dev_err(&pdev->dev, "failed to allocate memory\n");
		return -ENOMEM;
	}

	
//	pwm->clk = clk_get(&pdev->dev, NULL);
    pwm->clk = clk_get_sys("pwm", NULL);
	if (IS_ERR(pwm->clk)) {
		ret = PTR_ERR(pwm->clk);
		goto err_free;
    }
	pwm->clk_enabled = 0;

	pwm->use_count = 0;
	pwm->pwm_id = pdev->id;
	pwm->pdev = pdev;
//    dev_err(&pdev->dev, "pwm_id = %d \n",pwm->pwm_id);

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (r == NULL) {
		dev_err(&pdev->dev, "no memory resource defined\n");
		ret = -ENODEV;
		goto err_free_clk;
	}
//   dev_err(&pdev->dev, "resource %x\n", r->start);

	r = request_mem_region(r->start, resource_size(r), pdev->name);
	if (r == NULL) {
		dev_err(&pdev->dev, "failed to request memory resource\n");
		ret = -EBUSY;
		goto err_free_clk;
	}

	pwm->mmio_base = ioremap(r->start, resource_size(r));
	if (pwm->mmio_base == NULL) {
		dev_err(&pdev->dev, "failed to ioremap() registers\n");
		ret = -ENODEV;
		goto err_free_mem;
	}

	__add_pwm(pwm);

	platform_set_drvdata(pdev, pwm);


	tegra_periph_reset_assert(pwm->clk);
	msleep(1);
	tegra_periph_reset_deassert(pwm->clk);
        msleep(1);

	return 0;

err_free_mem:
	release_mem_region(r->start, resource_size(r));
err_free_clk:
	clk_put(pwm->clk);
err_free:
	kfree(pwm);
	return ret;
}

static int __devexit pwm_remove(struct platform_device *pdev)
{
	struct pwm_device *pwm;
	struct resource *r;

	pwm = platform_get_drvdata(pdev);
	if (pwm == NULL)
		return -ENODEV;

	mutex_lock(&pwm_lock);
	list_del(&pwm->node);
	mutex_unlock(&pwm_lock);

	iounmap(pwm->mmio_base);

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	//WBT #196339
	if (r == NULL) {
		dev_err(&pdev->dev, "failed to request memory resource\n");
		return -ENODEV;
	}

	release_mem_region(r->start, resource_size(r));

	clk_put(pwm->clk);
	kfree(pwm);
	return 0;
}

static struct platform_driver pwm_driver = {
	.driver		= {
		.name	= "tegra-pwm",
		.owner	= THIS_MODULE,
	},
	.probe		= pwm_probe,
	.remove		= __devexit_p(pwm_remove),
};

static int __init pwm_init(void)
{
	return platform_driver_register(&pwm_driver);
}
arch_initcall(pwm_init);

static void __exit pwm_exit(void)
{
	platform_driver_unregister(&pwm_driver);
}
module_exit(pwm_exit);

MODULE_LICENSE("GPL v2");
