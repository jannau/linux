/**
	@brief		 S5K5BAFX -samsung s5k5bafx 2M CMOS sensor driver setting value
	@author		 hyungmoo.huh@lge.com
	@date		 2010.11.19
*/


#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include "s5k5bafx.h"
#include <mach/gpio-names.h>
#include <mach/hardware.h>
#include <linux/gpio.h>
#include "../../../startablet/star_cam_pmic.h"
#define VC_CAMERA_CROP

const i2c_cam_reg16 S5k5bafx_init_1280x720[] =
{
	{0x0028, 0xD000},
	{0x002A, 0x1030},
	{0x0F12, 0x0000},
	{0x002A, 0x0014},
	{0x0F12, 0x0001},

	{0xFFFE, 0x0014},// Delay 100 -> 20

	{0x0028, 0x7000},
	{0x002A, 0x2080},
	{0x0F12, 0xB510},	// 700020800
	{0x0F12, 0xF000},	// 700020822
	{0x0F12, 0xF8FC},	// 700020844
	{0x0F12, 0xBC10},	// 700020866
	{0x0F12, 0xBC08},	// 700020888
	{0x0F12, 0x4718},	// 7000208AA
	{0x0F12, 0xB5FE},	// 7000208CC
	{0x0F12, 0x0007},	// 7000208EE
	{0x0F12, 0x683C},	// 700020900
	{0x0F12, 0x687E},	// 700020922
	{0x0F12, 0x1DA5},	// 700020944
	{0x0F12, 0x88A0},	// 700020966
	{0x0F12, 0x2800},	// 700020988
	{0x0F12, 0xD00B},	// 7000209AA
	{0x0F12, 0x88A8},	// 7000209CC
	{0x0F12, 0x2800},	// 7000209EE
	{0x0F12, 0xD008},	// 700020A00
	{0x0F12, 0x8820},	// 700020A2               2
	{0x0F12, 0x8829},	// 700020A4               4
	{0x0F12, 0x4288},	// 700020A6               6
	{0x0F12, 0xD301},	// 700020A8               8
	{0x0F12, 0x1A40},	// 700020AA               A
	{0x0F12, 0xE000},	// 700020AC               C
	{0x0F12, 0x1A08},	// 700020AE               E
	{0x0F12, 0x9001},	// 700020B0               0
	{0x0F12, 0xE001},	// 700020B2               2
	{0x0F12, 0x2019},	// 700020B4               4
	{0x0F12, 0x9001},	// 700020B6               6
	{0x0F12, 0x497B},	// 700020B8               8
	{0x0F12, 0x466B},	// 700020BA               A
	{0x0F12, 0x8A48},	// 700020BC               C
	{0x0F12, 0x8118},	// 700020BE               E
	{0x0F12, 0x8A88},	// 700020C0               0
	{0x0F12, 0x8158},	// 700020C2               2
	{0x0F12, 0x4879},	// 700020C4               4
	{0x0F12, 0x8940},	// 700020C6               6
	{0x0F12, 0x0040},	// 700020C8               8
	{0x0F12, 0x2103},	// 700020CA               A
	{0x0F12, 0xF000},	// 700020CC               C
	{0x0F12, 0xF902},	// 700020CE               E
	{0x0F12, 0x88A1},	// 700020D0               0
	{0x0F12, 0x4288},	// 700020D2               2
	{0x0F12, 0xD908},	// 700020D4               4
	{0x0F12, 0x8828},	// 700020D6               6
	{0x0F12, 0x8030},	// 700020D8               8
	{0x0F12, 0x8868},	// 700020DA               A
	{0x0F12, 0x8070},	// 700020DC               C
	{0x0F12, 0x88A8},	// 700020DE               E
	{0x0F12, 0x6038},	// 700020E0               0
	{0x0F12, 0xBCFE},	// 700020E2               2
	{0x0F12, 0xBC08},	// 700020E4               4
	{0x0F12, 0x4718},	// 700020E6               6
	{0x0F12, 0x88A9},	// 700020E8               8
	{0x0F12, 0x4288},	// 700020EA               A
	{0x0F12, 0xD906},	// 700020EC               C
	{0x0F12, 0x8820},	// 700020EE               E
	{0x0F12, 0x8030},	// 700020F0               0
	{0x0F12, 0x8860},	// 700020F2               2
	{0x0F12, 0x8070},	// 700020F4               4
	{0x0F12, 0x88A0},	// 700020F6               6
	{0x0F12, 0x6038},	// 700020F8               8
	{0x0F12, 0xE7F2},	// 700020FA               A
	{0x0F12, 0x9801},	// 700020FC               C
	{0x0F12, 0xA902},	// 700020FE               E
	{0x0F12, 0xF000},	// 70002100               0
	{0x0F12, 0xF8EE},	// 70002102               2
	{0x0F12, 0x0033},	// 70002104               4
	{0x0F12, 0x0029},	// 70002106               6
	{0x0F12, 0x9A02},	// 70002108               8
	{0x0F12, 0x0020},	// 7000210A               A
	{0x0F12, 0xF000},	// 7000210C               C
	{0x0F12, 0xF8F0},	// 7000210E               E
	{0x0F12, 0x6038},	// 70002110               0
	{0x0F12, 0xE7E6},	// 70002112               2
	{0x0F12, 0xB5F0},	// 70002114               4
	{0x0F12, 0xB08B},	// 70002116               6
	{0x0F12, 0x0006},	// 70002118               8
	{0x0F12, 0x2000},	// 7000211A               A
	{0x0F12, 0x9004},	// 7000211C               C
	{0x0F12, 0x6835},	// 7000211E               E
	{0x0F12, 0x6874},	// 70002120               0
	{0x0F12, 0x68B0},	// 70002122               2
	{0x0F12, 0x900A},	// 70002124               4
	{0x0F12, 0x68F0},	// 70002126               6
	{0x0F12, 0x9009},	// 70002128               8
	{0x0F12, 0x4F60},	// 7000212A               A
	{0x0F12, 0x8979},	// 7000212C               C
	{0x0F12, 0x084A},	// 7000212E               E
	{0x0F12, 0x88A8},	// 70002130               0
	{0x0F12, 0x88A3},	// 70002132               2
	{0x0F12, 0x4298},	// 70002134               4
	{0x0F12, 0xD300},	// 70002136               6
	{0x0F12, 0x0018},	// 70002138               8
	{0x0F12, 0xF000},	// 7000213A               A
	{0x0F12, 0xF8E1},	// 7000213C               C
	{0x0F12, 0x9007},	// 7000213E               E
	{0x0F12, 0x0021},	// 70002140               0
	{0x0F12, 0x0028},	// 70002142               2
	{0x0F12, 0xAA04},	// 70002144               4
	{0x0F12, 0xF000},	// 70002146               6
	{0x0F12, 0xF8E3},	// 70002148               8
	{0x0F12, 0x9006},	// 7000214A               A
	{0x0F12, 0x88A8},	// 7000214C               C
	{0x0F12, 0x2800},	// 7000214E               E
	{0x0F12, 0xD102},	// 70002150               0
	{0x0F12, 0x27FF},	// 70002152               2
	{0x0F12, 0x1C7F},	// 70002154               4
	{0x0F12, 0xE047},	// 70002156               6
	{0x0F12, 0x88A0},	// 70002158               8
	{0x0F12, 0x2800},	// 7000215A               A
	{0x0F12, 0xD101},	// 7000215C               C
	{0x0F12, 0x2700},	// 7000215E               E
	{0x0F12, 0xE042},	// 70002160               0
	{0x0F12, 0x8820},	// 70002162               2
	{0x0F12, 0x466B},	// 70002164               4
	{0x0F12, 0x8198},	// 70002166               6
	{0x0F12, 0x8860},	// 70002168               8
	{0x0F12, 0x81D8},	// 7000216A               A
	{0x0F12, 0x8828},	// 7000216C               C
	{0x0F12, 0x8118},	// 7000216E               E
	{0x0F12, 0x8868},	// 70002170               0
	{0x0F12, 0x8158},	// 70002172               2
	{0x0F12, 0xA802},	// 70002174               4
	{0x0F12, 0xC803},	// 70002176               6
	{0x0F12, 0xF000},	// 70002178               8
	{0x0F12, 0xF8D2},	// 7000217A               A
	{0x0F12, 0x9008},	// 7000217C               C
	{0x0F12, 0x8ABA},	// 7000217E               E
	{0x0F12, 0x9808},	// 70002180               0
	{0x0F12, 0x466B},	// 70002182               2
	{0x0F12, 0x4342},	// 70002184               4
	{0x0F12, 0x9202},	// 70002186               6
	{0x0F12, 0x8820},	// 70002188               8
	{0x0F12, 0x8198},	// 7000218A               A
	{0x0F12, 0x8860},	// 7000218C               C
	{0x0F12, 0x81D8},	// 7000218E               E
	{0x0F12, 0x980A},	// 70002190               0
	{0x0F12, 0x9903},	// 70002192               2
	{0x0F12, 0xF000},	// 70002194               4
	{0x0F12, 0xF8C4},	// 70002196               6
	{0x0F12, 0x9A02},	// 70002198               8
	{0x0F12, 0x17D1},	// 7000219A               A
	{0x0F12, 0x0E09},	// 7000219C               C
	{0x0F12, 0x1889},	// 7000219E               E
	{0x0F12, 0x1209},	// 700021A0               0
	{0x0F12, 0x4288},	// 700021A2               2
	{0x0F12, 0xDD1F},	// 700021A4               4
	{0x0F12, 0x8820},	// 700021A6               6
	{0x0F12, 0x466B},	// 700021A8               8
	{0x0F12, 0x8198},	// 700021AA               A
	{0x0F12, 0x8860},	// 700021AC               C
	{0x0F12, 0x81D8},	// 700021AE               E
	{0x0F12, 0x980A},	// 700021B0               0
	{0x0F12, 0x9903},	// 700021B2               2
	{0x0F12, 0xF000},	// 700021B4               4
	{0x0F12, 0xF8B4},	// 700021B6               6
	{0x0F12, 0x9001},	// 700021B8               8
	{0x0F12, 0x8828},	// 700021BA               A
	{0x0F12, 0x466B},	// 700021BC               C
	{0x0F12, 0x8118},	// 700021BE               E
	{0x0F12, 0x8868},	// 700021C0               0
	{0x0F12, 0x8158},	// 700021C2               2
	{0x0F12, 0x980A},	// 700021C4               4
	{0x0F12, 0x9902},	// 700021C6               6
	{0x0F12, 0xF000},	// 700021C8               8
	{0x0F12, 0xF8AA},	// 700021CA               A
	{0x0F12, 0x8AB9},	// 700021CC               C
	{0x0F12, 0x9A08},	// 700021CE               E
	{0x0F12, 0x4351},	// 700021D0               0
	{0x0F12, 0x17CA},	// 700021D2               2
	{0x0F12, 0x0E12},	// 700021D4               4
	{0x0F12, 0x1851},	// 700021D6               6
	{0x0F12, 0x120A},	// 700021D8               8
	{0x0F12, 0x9901},	// 700021DA               A
	{0x0F12, 0xF000},	// 700021DC               C
	{0x0F12, 0xF890},	// 700021DE               E
	{0x0F12, 0x0407},	// 700021E0               0
	{0x0F12, 0x0C3F},	// 700021E2               2
	{0x0F12, 0xE000},	// 700021E4               4
	{0x0F12, 0x2700},	// 700021E6               6
	{0x0F12, 0x8820},	// 700021E8               8
	{0x0F12, 0x466B},	// 700021EA               A
	{0x0F12, 0xAA05},	// 700021EC               C
	{0x0F12, 0x8198},	// 700021EE               E
	{0x0F12, 0x8860},	// 700021F0               0
	{0x0F12, 0x81D8},	// 700021F2               2
	{0x0F12, 0x8828},	// 700021F4               4
	{0x0F12, 0x8118},	// 700021F6               6
	{0x0F12, 0x8868},	// 700021F8               8
	{0x0F12, 0x8158},	// 700021FA               A
	{0x0F12, 0xA802},	// 700021FC               C
	{0x0F12, 0xC803},	// 700021FE               E
	{0x0F12, 0x003B},	// 70002200               0
	{0x0F12, 0xF000},	// 70002202               2
	{0x0F12, 0xF895},	// 70002204               4
	{0x0F12, 0x88A1},	// 70002206               6
	{0x0F12, 0x88A8},	// 70002208               8
	{0x0F12, 0x003A},	// 7000220A               A
	{0x0F12, 0xF000},	// 7000220C               C
	{0x0F12, 0xF898},	// 7000220E               E
	{0x0F12, 0x0004},	// 70002210               0
	{0x0F12, 0xA804},	// 70002212               2
	{0x0F12, 0xC803},	// 70002214               4
	{0x0F12, 0x9A09},	// 70002216               6
	{0x0F12, 0x9B07},	// 70002218               8
	{0x0F12, 0xF000},	// 7000221A               A
	{0x0F12, 0xF889},	// 7000221C               C
	{0x0F12, 0xA806},	// 7000221E               E
	{0x0F12, 0xC805},	// 70002220               0
	{0x0F12, 0x0021},	// 70002222               2
	{0x0F12, 0xF000},	// 70002224               4
	{0x0F12, 0xF88C},	// 70002226               6
	{0x0F12, 0x6030},	// 70002228               8
	{0x0F12, 0xB00B},	// 7000222A               A
	{0x0F12, 0xBCF0},	// 7000222C               C
	{0x0F12, 0xBC08},	// 7000222E               E
	{0x0F12, 0x4718},	// 70002230               0
	{0x0F12, 0xB510},	// 70002232               2
	{0x0F12, 0x6800},	// 70002234               4
	{0x0F12, 0x2805},	// 70002236               6
	{0x0F12, 0xD11E},	// 70002238               8
	{0x0F12, 0x481D},	// 7000223A               A
	{0x0F12, 0x491D},	// 7000223C               C
	{0x0F12, 0x8800},	// 7000223E               E
	{0x0F12, 0x8348},	// 70002240               0
	{0x0F12, 0x2301},	// 70002242               2
	{0x0F12, 0x31A0},	// 70002244               4
	{0x0F12, 0x720B},	// 70002246               6
	{0x0F12, 0x2000},	// 70002248               8
	{0x0F12, 0x000A},	// 7000224A               A
	{0x0F12, 0x3A60},	// 7000224C               C
	{0x0F12, 0x7150},	// 7000224E               E
	{0x0F12, 0x71CB},	// 70002250               0
	{0x0F12, 0x22FF},	// 70002252               2
	{0x0F12, 0x3980},	// 70002254               4
	{0x0F12, 0x75CA},	// 70002256               6
	{0x0F12, 0x764A},	// 70002258               8
	{0x0F12, 0x000A},	// 7000225A               A
	{0x0F12, 0x3260},	// 7000225C               C
	{0x0F12, 0x7210},	// 7000225E               E
	{0x0F12, 0x7250},	// 70002260               0
	{0x0F12, 0x2008},	// 70002262               2
	{0x0F12, 0x7488},	// 70002264               4
	{0x0F12, 0x74C8},	// 70002266               6
	{0x0F12, 0x4813},	// 70002268               8
	{0x0F12, 0x217E},	// 7000226A               A
	{0x0F12, 0x43C9},	// 7000226C               C
	{0x0F12, 0x8101},	// 7000226E               E
	{0x0F12, 0x2306},	// 70002270               0
	{0x0F12, 0x5EC0},	// 70002272               2
	{0x0F12, 0xF000},	// 70002274               4
	{0x0F12, 0xF86C},	// 70002276               6
	{0x0F12, 0xBC10},	// 70002278               8
	{0x0F12, 0xBC08},	// 7000227A               A
	{0x0F12, 0x4718},	// 7000227C               C
	{0x0F12, 0xB570},	// 7000227E               E
	{0x0F12, 0x2200},	// 70002280               0
	{0x0F12, 0x490E},	// 70002282               2
	{0x0F12, 0x480E},	// 70002284               4
	{0x0F12, 0x2401},	// 70002286               6
	{0x0F12, 0xF000},	// 70002288               8
	{0x0F12, 0xF86A},	// 7000228A               A
	{0x0F12, 0x0022},	// 7000228C               C
	{0x0F12, 0x490D},	// 7000228E               E
	{0x0F12, 0x480D},	// 70002290               0
	{0x0F12, 0x2502},	// 70002292               2
	{0x0F12, 0xF000},	// 70002294               4
	{0x0F12, 0xF864},	// 70002296               6
	{0x0F12, 0x490C},	// 70002298               8
	{0x0F12, 0x480D},	// 7000229A               A
	{0x0F12, 0x002A},	// 7000229C               C
	{0x0F12, 0xF000},	// 7000229E               E
	{0x0F12, 0xF85F},	// 700022A0               0
	{0x0F12, 0xBC70},	// 700022A2               2
	{0x0F12, 0xBC08},	// 700022A4               4
	{0x0F12, 0x4718},	// 700022A6               6
	{0x0F12, 0x1A28},	// 700022A8               8
	{0x0F12, 0x7000},	// 700022AA               A
	{0x0F12, 0x0D64},	// 700022AC               C
	{0x0F12, 0x7000},	// 700022AE               E
	{0x0F12, 0x2370},	// 700022B0               0
	{0x0F12, 0x7000},	// 700022B2               2
	{0x0F12, 0x1C14},	// 700022B4               4
	{0x0F12, 0x7000},	// 700022B6               6
	{0x0F12, 0x14FA},	// 700022B8               8
	{0x0F12, 0x7000},	// 700022BA               A
	{0x0F12, 0x2115},	// 700022BC               C
	{0x0F12, 0x7000},	// 700022BE               E
	{0x0F12, 0x622F},	// 700022C0               0
	{0x0F12, 0x0000},	// 700022C2               2
	{0x0F12, 0x208D},	// 700022C4               4
	{0x0F12, 0x7000},	// 700022C6               6
	{0x0F12, 0x6445},	// 700022C8               8
	{0x0F12, 0x0000},	// 700022CA               A
	{0x0F12, 0x2233},	// 700022CC               C
	{0x0F12, 0x7000},	// 700022CE               E
	{0x0F12, 0x3B5B},	// 700022D0               0
	{0x0F12, 0x0000},	// 700022D2               2
	{0x0F12, 0x4778},	// 700022D4               4
	{0x0F12, 0x46C0},	// 700022D6               6
	{0x0F12, 0xF004},	// 700022D8               8
	{0x0F12, 0xE51F},	// 700022DA               A
	{0x0F12, 0xA464},	// 700022DC               C
	{0x0F12, 0x0000},	// 700022DE               E
	{0x0F12, 0x4778},	// 700022E0               0
	{0x0F12, 0x46C0},	// 700022E2               2
	{0x0F12, 0xC000},	// 700022E4               4
	{0x0F12, 0xE59F},	// 700022E6               6
	{0x0F12, 0xFF1C},	// 700022E8               8
	{0x0F12, 0xE12F},	// 700022EA               A
	{0x0F12, 0x6009},	// 700022EC               C
	{0x0F12, 0x0000},	// 700022EE               E
	{0x0F12, 0x4778},	// 700022F0               0
	{0x0F12, 0x46C0},	// 700022F2               2
	{0x0F12, 0xC000},	// 700022F4               4
	{0x0F12, 0xE59F},	// 700022F6               6
	{0x0F12, 0xFF1C},	// 700022F8               8
	{0x0F12, 0xE12F},	// 700022FA               A
	{0x0F12, 0x622F},	// 700022FC               C
	{0x0F12, 0x0000},	// 700022FE               E
	{0x0F12, 0x4778},	// 70002300               0
	{0x0F12, 0x46C0},	// 70002302               2
	{0x0F12, 0xC000},	// 70002304               4
	{0x0F12, 0xE59F},	// 70002306               6
	{0x0F12, 0xFF1C},	// 70002308               8
	{0x0F12, 0xE12F},	// 7000230A               A
	{0x0F12, 0x5F49},	// 7000230C               C
	{0x0F12, 0x0000},	// 7000230E               E
	{0x0F12, 0x4778},	// 70002310               0
	{0x0F12, 0x46C0},	// 70002312               2
	{0x0F12, 0xC000},	// 70002314               4
	{0x0F12, 0xE59F},	// 70002316               6
	{0x0F12, 0xFF1C},	// 70002318               8
	{0x0F12, 0xE12F},	// 7000231A               A
	{0x0F12, 0x5FC7},	// 7000231C               C
	{0x0F12, 0x0000},	// 7000231E               E
	{0x0F12, 0x4778},	// 70002320               0
	{0x0F12, 0x46C0},	// 70002322               2
	{0x0F12, 0xC000},	// 70002324               4
	{0x0F12, 0xE59F},	// 70002326               6
	{0x0F12, 0xFF1C},	// 70002328               8
	{0x0F12, 0xE12F},	// 7000232A               A
	{0x0F12, 0x5457},	// 7000232C               C
	{0x0F12, 0x0000},	// 7000232E               E
	{0x0F12, 0x4778},	// 70002330               0
	{0x0F12, 0x46C0},	// 70002332               2
	{0x0F12, 0xC000},	// 70002334               4
	{0x0F12, 0xE59F},	// 70002336               6
	{0x0F12, 0xFF1C},	// 70002338               8
	{0x0F12, 0xE12F},	// 7000233A               A
	{0x0F12, 0x5FA3},	// 7000233C               C
	{0x0F12, 0x0000},	// 7000233E               E
	{0x0F12, 0x4778},	// 70002340               0
	{0x0F12, 0x46C0},	// 70002342               2
	{0x0F12, 0xC000},	// 70002344               4
	{0x0F12, 0xE59F},	// 70002346               6
	{0x0F12, 0xFF1C},	// 70002348               8
	{0x0F12, 0xE12F},	// 7000234A               A
	{0x0F12, 0x51F9},	// 7000234C               C
	{0x0F12, 0x0000},	// 7000234E               E
	{0x0F12, 0x4778},	// 70002350               0
	{0x0F12, 0x46C0},	// 70002352               2
	{0x0F12, 0xC000},	// 70002354               4
	{0x0F12, 0xE59F},	// 70002356               6
	{0x0F12, 0xFF1C},	// 70002358               8
	{0x0F12, 0xE12F},	// 7000235A               A
	{0x0F12, 0x7633},	// 7000235C               C
	{0x0F12, 0x0000},	// 7000235E               E
	{0x0F12, 0x4778},	// 70002360               0
	{0x0F12, 0x46C0},	// 70002362               2
	{0x0F12, 0xC000},	// 70002364               4
	{0x0F12, 0xE59F},	// 70002366               6
	{0x0F12, 0xFF1C},	// 70002368               8
	{0x0F12, 0xE12F},	// 7000236A               A
	{0x0F12, 0xA007},	// 7000236C               C
	{0x0F12, 0x0000},	// 7000236E               E
	{0x0F12, 0x0360},	// 70002370               0
	{0x0F12, 0x6F4E},	// 70002372               2
	{0x0F12, 0x2076},	// 70002374               4
	{0x0F12, 0x3831},	// 70002376               6
	{0x0F12, 0x3220},	// 70002378               8
	{0x0F12, 0x3030},	// 7000237A               A
	{0x0F12, 0x0038},	// 7000237C               C
	{0x0F12, 0xE010},	// 7000237E               E
	{0x0F12, 0x0208},	// 70002380               0
	{0x0F12, 0x0038},	// 70002382               2
	// End of Trap and Patch (Last : 70002382h) // Total Size 772 (0x0304)
	{0x0028, 0xD000},
	{0x002A, 0x1000},
	{0x0F12, 0x0001},

	{0x0028, 0x7000},
	{0x002A, 0x0ADC},
	{0x0F12, 0x0AF0},	//#setot_uOnlineClocksDiv4OnlineC0             //2800
	{0x002A, 0x0AE2},
	{0x0F12, 0x222E},	//#setot_usSetRomWaitStatesSetRomThreshold4KHz //ROM Wait Threshold to 35MHz
	{0x002A, 0x0B94},
	{0x0F12, 0x0500},	//#awbb_GainsInit_0_:R    insInit
	{0x0F12, 0x0400},	//#awbb_GainsInit_1_:G    insInit
	{0x0F12, 0x0700},	//#awbb_GainsInit_2_:B    insInit
	{0x002A, 0x04A0},
	{0x0F12, 0x8000},	//#lt_uLeiInit:AE start   Init:AE
	{0x002A, 0x049A},
	{0x0F12, 0x003B},	//#lt_uMinExp             Exp



	//=================================================================================================
	//	Set CIS/APS/Analog
	//=================================================================================================
	// This registers are for FACTORY ONLY. If you change it without prior notification,
	// YOU are RESPONSIBLE for the FAILURE that will happen in the future.
	//=================================================================================================
	{0x0028, 0xD000},
	{0x002A, 0xF106},
	{0x0F12, 0x0001},	// L-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xF206},
	{0x0F12, 0x0001},	// F-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xC202},
	{0x0F12, 0x0700},	// tgr_coarse_integration_time[15:0]: revised by Ana 080115
	{0x002A, 0xF414},
	{0x0F12, 0x0030},	// aig_adc_sat[7:0] (aig_adc_sat[7:4]+1)*33mV + 690mV
	{0x002A, 0xC204},
	{0x0F12, 0x0100},	// tgr_analogue_code_global[12:0] Analog gain X8
	{0x002A, 0xF402},
	{0x0F12, 0x0092},	// aig_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_offset2[7:0]: revised by Ana 080425
	{0x002A, 0xF700},
	{0x0F12, 0x0040},	// bpradlc_control[7:0]: revised by Ana 080529 // bpradlc_nactive_pedestal[7:5],bpradlc_passthrough[1],bpradlc_bypass[0]
	{0x002A, 0xF708},
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_r[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gr[7:0]: revised by Tg 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gb[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_b[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_total[7:0]: revised by TG 080529
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_r[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gr[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gb[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_b[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_total[7:0]: revised by Ana 080425
	{0x0F12, 0x0001},	// bpradlc_adlc_en[7:0]: revised by TG 080529	// bpradlc_f_l_adlc_en[2],bpradlc_f_adlc_en[1],bpradlc_h_adlc_en[0]
	{0x0F12, 0x0015},	// bpradlc_adlc_option[7:0]: revised by TG 080425 // bpradlc_h_adlc_ch_sel[4], // bpradlc_max_data_clip_sel[3:2],bpradlc_f_adlc_max_data_clip_sel[1:0]
	{0x0F12, 0x0001},	// bpradlc_adlc_bpr_en[7:0] adlc_bpr_enable: revised by Ana 080112
	{0x0F12, 0x0040},	// bpradlc_adlc_bpr_thresh[7:0]: 080425
	{0x002A, 0xF48A},
	{0x0F12, 0x0048},	// aig_ld_ptr7[9:0] line OB start addr(04h~48h): revised by Ana 080911
	{0x002A, 0xF10A},
	{0x0F12, 0x008B},	// tgr_h_desc0_addr_end[10:0] line OB end addr(47h~8Bh): revised by Ana 080911 // line OB start - line OB end = 67d
	{0x002A, 0xF900},
	{0x0F12, 0x0067},	// cfpn_config: revised by Ana 080425 // cfpn_ref_gain[9:7],cpfn_ref_lines[6:5],cfpn_hbinning[4],cfpn_output_direct[3],// cfpn_overflow_protect[2],cpfn_passthrough[1],cfpn_bypass[0]
	{0x002A, 0xF406},
	{0x0F12, 0x0092},	// aig_cfpn_ref_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_cfpn_ref_offset2[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_cfpn_ref_gain[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_ld_ctrl[1:0] aig_ldb_en[1], aig_ld_en[0]
	{0x0F12, 0x0000},	// aig_row_id_ctrl[1:0]
	{0x002A, 0xF442},
	{0x0F12, 0x0000},	// aig_vavg[0]
	{0x0F12, 0x0000},	// aig_havg[0]=1b @H-avg mode: revised by Ana 080116
	{0x002A, 0xF448},
	{0x0F12, 0x0000},	// aig_sl_off[0]
	{0x002A, 0xF454},
	{0x0F12, 0x0001},	//Power save enable
	{0x0F12, 0x0001},	// aig_blst_en[0]
	{0x0F12, 0x0010},	// aig_blst_en_cintr[15:0]
	{0x0F12, 0x0000},	// aig_dshut_en[0]
	{0x002A, 0xF41A},
	{0x0F12, 0x00FF},	// aig_comp_bias[7:0] aig_comp2_bias[7:4], aig_comp1_bias[3:0]: revised by Ana 081013
	{0x0F12, 0x0003},	// aig_pix_bias[3:0]
	{0x002A, 0xF420},
	{0x0F12, 0x0030},	// aig_clp_lvl[7:0]: revised by Ana 080910(refer to 6AA)
	{0x002A, 0xF410},
	{0x0F12, 0x0001},	// aig_clp_sl_ctrl[0]
	{0x0F12, 0x0000},	// aig_cds_test[7:0], aig_cds_test[1]=1b @H-avg mode current save: revised by Ana 080116
	{0x002A, 0xF416},
	{0x0F12, 0x0001},	// aig_rmp_option[7:0]
	{0x002A, 0xF424},
	{0x0F12, 0x0000},	// aig_ref_option[7:0], aig_ref_option[0]=1b @H-avg mode odd cds off: revised by Ana 080116
	{0x002A, 0xF422},
	{0x0F12, 0x0000},	// aig_monit[7:0]
	{0x002A, 0xF41E},
	{0x0F12, 0x0000},	// aig_pd_pix[0]
	{0x002A, 0xF428},
	{0x0F12, 0x0000},	// aig_pd_cp[1:0] aig_pd_ncp[1], aig_pd_cp[0]
	{0x0F12, 0x0000},	// aig_pd_reg_pix[0]
	{0x0F12, 0x0000},	// aig_pd_reg_rg[0]
	{0x002A, 0xF430},
	{0x0F12, 0x0000},	// aig_pd_reg_tgsl[0]
	{0x0F12, 0x0000},	// aig_pd_reg_ntg[0]
	{0x0F12, 0x0008},	// aig_rosc_tune_cp[3:0]: revised by Ana 080418
	{0x0F12, 0x0005},	// aig_rosc_tune_ncp[3:0]
	{0x0F12, 0x000F},	// aig_cp_capa[3:0]
	{0x0F12, 0x0001},	// aig_reg_tune_pix[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_rg[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_tgsl[7:0]
	{0x0F12, 0x0010},	// aig_reg_tune_ntg[7:0] 0040 -> 0010
	{0x002A, 0xF4D6},
	{0x0F12, 0x00C0},	// aig_v15_tune[7:0]: revised by Ana 081010 // aig_v15_tune[7:4]=7h @sensor only Mode // aig_v15_tune[7:4]=9h @ISP 7.5fps Mode // aig_v15_tune[7:4]=Dh @ISP 15fps Mode
	{0x0F12, 0x0000},	//aig_vreg_sel[7:0] [9]h_test, [1]reg_sw_stby, [0]aig_reg_sel
	{0x002A, 0xF47C},
	{0x0F12, 0x000C},	//aig_ld_ptr0[4:0]
	{0x0F12, 0x0000},	//aig_ld_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF49A},
	{0x0F12, 0x0008},	//aig_sla_ptr0[3:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_sla_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4A2},
	{0x0F12, 0x0008},	//aig_slb_ptr0[7:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_slb_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4B2},
	{0x0F12, 0x0013},	//aig_rxa_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxa_ptr1[9:0]: revised by Ana 081010
	{0x0F12, 0x0013},	//aig_rxb_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxb_ptr1[9:0]: revised by Ana 081010
	{0x002A, 0xF4AA},
	{0x0F12, 0x009B},	//aig_txa_ptr0[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txa_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_txb_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txb_ptr1[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF474},
	{0x0F12, 0x0017},	//aig_s3_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x005F},	//aig_s3_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0017},	//aig_s4_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x008F},	//aig_s4_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF48C},
	{0x0F12, 0x0017},	//aig_clp_sl_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_clp_sl_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF4C8},
	{0x0F12, 0x0163},	//aig_off_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0193},	//aig_rmp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF490},
	{0x0F12, 0x0191},	//aig_comp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF418},
	{0x0F12, 0x0083},	//aig_dbs_option[7:0]: revised by Ana 081010

	//For ESD Check
	{0x0028, 0x7000},
	{0x002A, 0x0132},
	{0x0F12, 0xAAAA},	//#define REG_FWpid (For ESD Check)
	// * note : ESD Check Method.
	//	1. if (7000.0132 != AAAA ) reset // ( S/W reg. check)
	//	2. if (D000.1002 != 0000 ) reset // (internal Exception)

	//Set FPN Gain Input
	{0x002A, 0x1176},
	{0x0F12, 0x0020},	// fpn_GainInput[0]
	{0x0F12, 0x0040},	// fpn_GainInput[1]
	{0x0F12, 0x0080},	// fpn_GainInput[2]
	{0x0F12, 0x0100},	// fpn_GainInput[3]
	{0x0F12, 0x0014},	// fpn_GainOutput[0]
	{0x0F12, 0x000A},	// fpn_GainOutput[1]
	{0x0F12, 0x0008},	// fpn_GainOutput[2]
	{0x0F12, 0x0004},	// fpn_GainOutput[3]

	// Set CFPN Canceller
	{0x002A, 0x116E},
	{0x0F12, 0x0000},	// OvflProtect
	{0x0F12, 0x0000},	// bypassThrough
	{0x0F12, 0x0000},	// bypass
	{0x0F12, 0x0002},	// fpn.CollectMethod	// 0 : Center	1 : Right	2: LEFT
	{0x002A, 0x0AE8},
	{0x0F12, 0x0000},	// setot_bSendFpnToISP = ??

	// Set Forbidden Area
	{0x002A, 0x10EE},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_usRegCount // 0 value --> not use aig table 3
	{0x002A, 0x10F2},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_pSenModesRegsArray3[0]		//real register 0xD000F45A
	{0x002A, 0x1152},
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[48]    //real register 0xD000F4BA
	{0x0F12, 0x0028},	// senHal_SenRegsModes3_pSenModesRegsArray3[49]    //real register 0xD000F4BC
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[50]    //real register 0xD000F4BE
	{0x002A, 0x1148},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[43]    //real register 0xD000F4B0
	{0x002A, 0x1144},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[41]    //real register 0xD000F4AC
	{0x002A, 0x1150},
	{0x0F12, 0x0148},	// senHal_SenRegsModes3_pSenModesRegsArray3[47]    //real register 0xD000F4B8

	// Set AE Target
	{0x002A, 0x0F4C},
	{0x0F12, 0x003D},	// #TVAR_ae_BrAve



	//=================================================================================================
	//	Set Frame Rate
	//=================================================================================================
	{0x002A, 0x0484},
	{0x0F12, 0x61A8},	// #lt_uMaxExp1 	// 50ms
	{0x0F12, 0x0000},
	{0x0F12, 0x86A0},	// #lt_uMaxExp2 	// 200ms
	{0x0F12, 0x0001},
	{0x0F12, 0x8214},	// #lt_uCapMaxExp1	// 66ms
	{0x0F12, 0x0000},
	{0x0F12, 0xD2F0},	// #lt_uCapMaxExp2	// 108ms
	{0x0F12, 0x0000},
	{0x0F12, 0x04C0},	// #lt_uMaxAnGain1	// x4.75
	{0x0F12, 0x0700},	// #lt_uMaxAnGain2	// x7.0
	{0x0F12, 0x0100},	// #lt_uMaxDigGain
	{0x002A, 0x0F52},
	{0x0F12, 0x0007},	// #ae_StatMode



	//=================================================================================================
	//	Set Flicker  Current Flicker = 7000.164A //Mon_AFC_usFlExpQunat		0001:Off 1045:60Hz	1388:50Hz
	//=================================================================================================

	//1. Auto Flicker 60Hz Start
	{0x002A, 0x0B2E},
	{0x0F12, 0x0001},	// #AFC_Default60Hz 	1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	{0x002A, 0x03F8},
	{0x0F12, 0x007F},	// #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////2. Auto Flicker 50Hz Start
	//WRITE	70000B2E	0000	// #AFC_Default60Hz		1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	//WRITE 700003F8	007F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////3. Manual Flicker 60Hz
	//WRITE 700003F8	005F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F
	//WRITE 700003D4	0002  // #REG_SF_USER_FlickerQuant	 //Flicker 50Hz:0001/60Hz:0002/off:0000
	//WRITE 700003D6	0001  // #REG_SF_USER_FlickerQuantChanged //if change 0001 write

	////4. Manual Flicker 50Hz
	//WRITE 700003F8	005F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F
	//WRITE 700003D4	0001  // #REG_SF_USER_FlickerQuant	 //Flicker 50Hz:0001/60Hz:0002/off:0000
	//WRITE 700003D6	0001  // #REG_SF_USER_FlickerQuantChanged //if change 0001 write

	////5. Flicker Off
	//WRITE 700003F8	005F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F
	//WRITE 700003D4	0000  // #REG_SF_USER_FlickerQuant	 //Flicker 50Hz:0001/60Hz:0002/off:0000
	//WRITE 700003D6	0001  // #REG_SF_USER_FlickerQuantChanged //if change 0001 write


	//WRITE 70000B36	0001  // #AFC_ManualQuant


	// added for test
	//WRITE #senHal_uMinColsAddAnalogBin       	 	0048		//Subsampling시 1-H min 값  1768로 조절하기 위한 Register
	//WRITE #setot_uOnlineClocksDiv40		 	  			0A28 		//2800
	//WRITE #setot_usSetRomWaitStateThreshold4KHz	222E 		//ROM Wait Threshold to 35MHz



	//=================================================================================================
	//	Set PLL
	//=================================================================================================
	// Input CLK
	{0x002A, 0x01B8},
	{0x0F12, 0x5DC0},	//5DC0	//#REG_TC_IPRM_InClockLSBs	//24 -> 24.5M Hz(20081208)
	{0x0F12, 0x0000},	//#REG_TC_IPRM_InClockMSBs

	// Target Sys CLK
	{0x002A, 0x01CC},
	{0x0F12, 0x1770},	// #REG_TC_IPRM_OpClk4KHz_0 			//24Mhz

	// Target PVI CLK
	{0x0F12, 0x2318},	// #REG_TC_IPRM_MinOutRate4KHz_0	//36Mhz
	{0x0F12, 0x2338},	// #REG_TC_IPRM_MaxOutRate4KHz_0

	// PLL ETC
	{0x002A, 0x01C6},
	{0x0F12, 0x0003},	//#REG_TC_IPRM_UseNPviClocks
	{0x0F12, 0x0000},	//#REG_TC_IPRM_UseNMipiClocks
	{0x002A, 0x01C0},
	{0x0F12, 0x0000},	//#EG_TC_IPRM_ValidVActiveLow
	{0x002A, 0x01DE},
	{0x0F12, 0x0001},	//#REG_TC_IPRM_UseRegsAPI
	{0x0F12, 0x0001},	//#REG_TC_IPRM_InitParamsUpdated
	{0x002A, 0x1084},
	{0x0F12, 0x0001},	//#senHal_bUseAnalogBinning //Avg S.S
	{0x002A, 0x10B6},
	{0x0F12, 0x0048},	//#senHal_uMinColsAddAnalogBin



	//=================================================================================================
	//	Set Preview Config
	//=================================================================================================

	//* Preview Cropping Code *//
	{0x002A,	0x020A},
//	{0x0F12,	0x0640},	//#REG_TC_GP_PrevZoomReqInputWidth
//	{0x0F12,	0x0384},	//#REG_TC_GP_PrevZoomReqInputHeight
	{0x0F12,	0x0500},	//#REG_TC_GP_PrevZoomReqInputWidth		//0x640 => 1600,  0x500 => 1280
	{0x0F12,	0x02D0},	//#REG_TC_GP_PrevZoomReqInputHeight		//0x384 => 900,   0x2D0 => 720
	{0x0F12,	0x0000},	//#REG_TC_GP_PrevZoomReqInputWidthOfs
	{0x0F12,	0x01C0},	//#REG_TC_GP_PrevZoomReqInputHeightOfs

	//* Zoom Update *//
	{0x002A,	0x021A},
	{0x0F12,	0x0001},	// #REG_TC_GP_Inputs_ChangeRequest : 0x021A


	{0x002A, 0x0242},
	{0x0F12, 0x0500}, //0x0280, //0x0140(320)->640, //090416 800 -> 320, 0x0320,	                 1280 -> 0x500  //0280  // #REG_0TC_PCFG_usWidth						// 640 -> 800 _20081209
	{0x0F12, 0x02D0}, //0x01e0, //0x00f0(240)->480, //090416 600 -> 240, 0x0258, 0x168 -> 360     720 -> 0x2D0	//01e0  // #REG_0TC_PCFG_usHeight						// 480 -> 600 _20081209
	{0x0F12, 0x0005},	// #REG_0TC_PCFG_Format 						// YUV output
	{0x0F12, 0x2338},	// #REG_0TC_PCFG_usMaxOut4KHzRate
	{0x0F12, 0x2318},	// #REG_0TC_PCFG_usMinOut4KHzRate
//	{0x0F12, 0x446C},	// #REG_0TC_PCFG_usMaxOut4KHzRate
//	{0x0F12, 0x444C},	// #REG_0TC_PCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	// #REG_0TC_PCFG_PVIMask   (x2: PCLK rising x0: PCLK Falling)
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_uClockInd
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_usFrTimeType
//	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType
	{0x0F12, 0x0002},	// #REG_0TC_PCFG_FrRateQualityType		(0 : bin mode, 2 : No-bin mode)
	{0x0F12, 0x04E2},	// #REG_0TC_PCFG_usMaxFrTimeMsecMult10 // 8fps
	{0x0F12, 0x03E8},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 10fps
//	{0x0F12, 0x014D},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 30fps
	{0x002A, 0x0262},
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uPrevMirror -- X/Y flip 20090112
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uCaptureMirror -- X/Y flip 20090112
	//WRITE 70000262 000F  //#REG_0TC_PCFG_uPrevMirror
	//WRITE 70000264 000F  //#REG_0TC_PCFG_uCaptureMirror


	// Apply preview config
	{0x002A, 0x021C},
	{0x0F12, 0x0000},	//#REG_TC_GP_ActivePrevConfig	  //preview config0
	{0x002A, 0x0220},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevOpenAfterChange  //config 변경적용
	{0x002A, 0x01F8},
	{0x0F12, 0x0001},	//#REG_TC_GP_NewConfigSync
	{0x002A, 0x021E},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevConfigChanged



	//=================================================================================================
	//	Set Capture Config
	//=================================================================================================
	// For 1600x1202 capture
	//WRITE 70000202	0640	// #REG_TC_GP_CapReqInputWidth
	//WRITE 70000204	04C0	// #REG_TC_GP_CapReqInputHeight			// Vsize = 1216
	//WRITE 70000206	0000	// #REG_TC_GP_CapInputWidthOfs
	//WRITE 70000208	0000	// #REG_TC_GP_CapInputHeightOfs
	//
	//WRITE 70000212	0640	// #REG_TC_GP_CapZoomReqInputWidth
	//WRITE 70000214	04C0	// #REG_TC_GP_CapZoomReqInputHeight	// Vsize = 1216
	//WRITE 70000216	0000	// #REG_TC_GP_CapZoomReqInputWidthOfs
	//WRITE 70000218	0000	// #REG_TC_GP_CapZoomReqInputHeightOfs
	//WRITE 7000021A	0001	// #REG_TC_GP_InputsChangeRequest
	//WRITE 700001F8	0001	// #REG_TC_GP_NewConfigSync
	//

	{0x002A, 0x0302},
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uCaptureMode
	{0x0F12, 0x0640},	//#REG_0TC_CCFG_usWidth
	{0x0F12, 0x04B0},	//#REG_0TC_CCFG_usHeight						// Vsize = 1216
	{0x0F12, 0x0005},	//#REG_0TC_CCFG_Format	   //5:YUV,9:JPEG
	{0x0F12, 0x2338},	//#REG_0TC_CCFG_usMaxOut4KHzRate
	{0x0F12, 0x2318},	//#REG_0TC_CCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	//#REG_0TC_CCFG_PVIMask (x2: PCLK rising	x0: PCLK Falling)
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uClockInd
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_usFrTimeType
	{0x0F12, 0x0002},	//#REG_0TC_CCFG_FrRateQualityType
	{0x0F12, 0x07D0},	//#REG_0TC_CCFG_usMaxFrTimeMsecMult10 // 5fps
	{0x0F12, 0x0535},	//#REG_0TC_CCFG_usMinFrTimeMsecMult10 // 7.5fps



	//=================================================================================================
	//	Set Periodic Mismatch (Use default value)
	//=================================================================================================
	//WRITE 70000780	0000 //FFF0  // #msm_uOffsetNoBin[0][0]
	//WRITE 70000782	0000 //FFF0  // #msm_uOffsetNoBin[0][1]
	//WRITE 70000784	0000 //FFEE  // #msm_uOffsetNoBin[1][0]
	//WRITE 70000786	0000 //FFE0  // #msm_uOffsetNoBin[1][1]
	//WRITE 70000788	0000 //FFF0  // #msm_uOffsetNoBin[2][0]
	//WRITE 7000078A	0000 //FFF0  // #msm_uOffsetNoBin[2][1]
	//WRITE 7000078C	0000 //FFEE  // #msm_uOffsetNoBin[3][0]
	//WRITE 7000078E	0000 //FFE0  // #msm_uOffsetNoBin[3][1]
	//WRITE 70000798  0000 //FFE0  // #msm_uOffsetBin[0][0]
	//WRITE 7000079A  0000 //FFEE  // #msm_uOffsetBin[0][1]
	//WRITE 7000079C  0000 //FFF0  // #msm_uOffsetBin[1][0]
	//WRITE 7000079E  0000 //FFFB  // #msm_uOffsetBin[1][1]



	//=================================================================================================
	//	Set AE Weights
	//=================================================================================================
	{0x002A, 0x0F5A},
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_0_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_1_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_2_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_3_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_4_
	{0x0F12, 0x0000},	//0201	// #ae_WeightTbl_16_5_
	{0x0F12, 0x0000},	//0102	// #ae_WeightTbl_16_6_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_7_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_8_
	{0x0F12, 0x0000},	//0202	// #ae_WeightTbl_16_9_
	{0x0F12, 0x0000},	//0202	// #ae_WeightTbl_16_10
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_11
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_12
	{0x0F12, 0x0101},	//0402	// #ae_WeightTbl_16_13
	{0x0F12, 0x0101},	//0204	// #ae_WeightTbl_16_14
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_15
	{0x0F12, 0x0201},	//0201	// #ae_WeightTbl_16_16
	{0x0F12, 0x0202},	//0504	// #ae_WeightTbl_16_17
	{0x0F12, 0x0102},	//0405	// #ae_WeightTbl_16_18
	{0x0F12, 0x0000},	//0102	// #ae_WeightTbl_16_19
	{0x0F12, 0x0201},	//0501	// #ae_WeightTbl_16_20
	{0x0F12, 0x0505},	//0505	// #ae_WeightTbl_16_21
	{0x0F12, 0x0102},	//0505	// #ae_WeightTbl_16_22
	{0x0F12, 0x0000},	//0105	// #ae_WeightTbl_16_23
	{0x0F12, 0x0201},	//0503	// #ae_WeightTbl_16_24
	{0x0F12, 0x0202},	//0505	// #ae_WeightTbl_16_25
	{0x0F12, 0x0102},	//0505	// #ae_WeightTbl_16_26
	{0x0F12, 0x0000},	//0305	// #ae_WeightTbl_16_27
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_28
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_29
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_30
	{0x0F12, 0x0000},	//0303	// #ae_WeightTbl_16_31



	//=================================================================================================
	//	Set GAS & CCM White Point
	//=================================================================================================
	// param_start	TVAR_ash_AwbAshCord
	{0x002A, 0x0704},
	{0x0F12, 0x0090},	//000E	// #TVAR_ash_AwbAshCord_0_
	{0x0F12, 0x00B3},	//0012	// #TVAR_ash_AwbAshCord_1_
	{0x0F12, 0x00C3},	//0019	// #TVAR_ash_AwbAshCord_2_
	{0x0F12, 0x00E5},	//0034	// #TVAR_ash_AwbAshCord_3_
	{0x0F12, 0x0116},	//0044	// #TVAR_ash_AwbAshCord_4_
	{0x0F12, 0x0198},	//0052	// #TVAR_ash_AwbAshCord_5_
	{0x0F12, 0x0200},	//0058	// #TVAR_ash_AwbAshCord_6_

	// param_start	wbt_AwbCcmCord
	{0x002A, 0x06F2},
	{0x0F12, 0x00B3},	//0016	// #SARR_AwbCcmCord_0_	Hor
	{0x0F12, 0x00E5},	//001E	// #SARR_AwbCcmCord_1_	IncaA
	{0x0F12, 0x0100},	//0024	// #SARR_AwbCcmCord_2_	WW
	{0x0F12, 0x0116},	//0038	// #SARR_AwbCcmCord_3_	CW
	{0x0F12, 0x0150},	//004C	// #SARR_AwbCcmCord_4_	D50
	{0x0F12, 0x0198},	//0058	// #SARR_AwbCcmCord_5_	D65

	// Set Target Brightness Control
	{0x002A, 0x103E},
	{0x0F12, 0x0000},	// #SARR_IllumType_0_
	{0x0F12, 0x0009},	// #SARR_IllumType_1_
	{0x0F12, 0x0018},	// #SARR_IllumType_2_
	{0x0F12, 0x0032},	// #SARR_IllumType_3_
	{0x0F12, 0x004A},	// #SARR_IllumType_4_
	{0x0F12, 0x0051},	// #SARR_IllumType_5_
	{0x0F12, 0x0056},	// #SARR_IllumType_6_
	{0x0F12, 0x010C},	// #SARe_2_R_IllumTypeF_0_
	{0x0F12, 0x010C},	// #SARe_3_R_IllumTypeF_1_
	{0x0F12, 0x0109},	// #SARe_4_R_IllumTypeF_2_
	{0x0F12, 0x0105},	// #SARe_5_R_IllumTypeF_3_
	{0x0F12, 0x0102},	// #SARe_6_R_IllumTypeF_4_
	{0x0F12, 0x00FB},	// #SARR_IllumTypeF_5_
	{0x0F12, 0x00F8},	// #SARR_IllumTypeF_6_

	// Indoor GAS Alpha
	{0x002A, 0x0712},
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[0]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[1]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[2]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[3]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[4]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[5]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[6]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[7]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[8]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[9]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[10]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[11]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[12]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[13]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[14]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[15]
	{0x0F12, 0x00E0},	// #TVAR_ash_GASAlpha[16]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[17]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[18]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[19]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[20]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[21]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[22]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[23]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[24]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[25]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[26]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[27]

	// Outdoor GAS Alpha
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_0_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_1_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_2_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_3_

	// GAS LUT Start Address
	{0x002A, 0x0754},
	{0x0F12, 0x2388},	//#TVAR_ash_pGAS
	{0x0F12, 0x7000},	//#TVAR_ash_pGAS

	// GAS LUT
	//							//1030	//1027
	{0x002A, 0x2388},
	{0x0F12, 0x01C9},	//#TVAR_ash_pGAS[0]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[1]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[2]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[3]
	{0x0F12, 0x0105},	//#TVAR_ash_pGAS[4]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[5]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[6]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[7]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[8]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[9]
	{0x0F12, 0x0160},	//#TVAR_ash_pGAS[10]
	{0x0F12, 0x0189},	//#TVAR_ash_pGAS[11]
	{0x0F12, 0x01D0},	//#TVAR_ash_pGAS[12]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[13]
	{0x0F12, 0x016A},	//#TVAR_ash_pGAS[14]
	{0x0F12, 0x0126},	//#TVAR_ash_pGAS[15]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[16]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[17]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[18]
	{0x0F12, 0x009B},	//#TVAR_ash_pGAS[19]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[20]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[21]
	{0x0F12, 0x00EF},	//#TVAR_ash_pGAS[22]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[23]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[24]
	{0x0F12, 0x019C},	//#TVAR_ash_pGAS[25]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[26]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[27]
	{0x0F12, 0x00E2},	//#TVAR_ash_pGAS[28]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[29]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[30]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[31]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[32]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[33]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[34]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[35]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[36]
	{0x0F12, 0x0133},	//#TVAR_ash_pGAS[37]
	{0x0F12, 0x016E},	//#TVAR_ash_pGAS[38]
	{0x0F12, 0x0140},	//#TVAR_ash_pGAS[39]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[40]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[41]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[42]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[43]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[44]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[45]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[46]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[47]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[48]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[49]
	{0x0F12, 0x010D},	//#TVAR_ash_pGAS[50]
	{0x0F12, 0x0157},	//#TVAR_ash_pGAS[51]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[52]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[53]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[54]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[55]
	{0x0F12, 0x002F},	//#TVAR_ash_pGAS[56]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[57]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[58]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[59]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[60]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[61]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[62]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[63]
	{0x0F12, 0x0142},	//#TVAR_ash_pGAS[64]
	{0x0F12, 0x011A},	//#TVAR_ash_pGAS[65]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[66]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[67]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[68]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[69]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[70]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[71]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[72]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[73]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[74]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[75]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[76]
	{0x0F12, 0x013B},	//#TVAR_ash_pGAS[77]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[78]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[79]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[80]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[81]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[82]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[83]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[84]
	{0x0F12, 0x0014},	//#TVAR_ash_pGAS[85]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[86]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[87]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[88]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[89]
	{0x0F12, 0x013F},	//#TVAR_ash_pGAS[90]
	{0x0F12, 0x012C},	//#TVAR_ash_pGAS[91]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[92]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[93]
	{0x0F12, 0x0067},	//#TVAR_ash_pGAS[94]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[95]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[96]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[97]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[98]
	{0x0F12, 0x004B},	//#TVAR_ash_pGAS[99]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[100]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[101]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[102]
	{0x0F12, 0x0150},	//#TVAR_ash_pGAS[103]
	{0x0F12, 0x0149},	//#TVAR_ash_pGAS[104]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[105]
	{0x0F12, 0x00C5},	//#TVAR_ash_pGAS[106]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[107]
	{0x0F12, 0x0061},	//#TVAR_ash_pGAS[108]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[109]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[110]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[111]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[112]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[113]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[114]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[115]
	{0x0F12, 0x016D},	//#TVAR_ash_pGAS[116]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[117]
	{0x0F12, 0x013A},	//#TVAR_ash_pGAS[118]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[119]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[120]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[121]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[122]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[123]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[124]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[125]
	{0x0F12, 0x00D8},	//#TVAR_ash_pGAS[126]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[127]
	{0x0F12, 0x0154},	//#TVAR_ash_pGAS[128]
	{0x0F12, 0x0186},	//#TVAR_ash_pGAS[129]
	{0x0F12, 0x018F},	//#TVAR_ash_pGAS[130]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[131]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[132]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[133]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[134]
	{0x0F12, 0x00B4},	//#TVAR_ash_pGAS[135]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[136]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[137]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[138]
	{0x0F12, 0x0112},	//#TVAR_ash_pGAS[139]
	{0x0F12, 0x014F},	//#TVAR_ash_pGAS[140]
	{0x0F12, 0x0185},	//#TVAR_ash_pGAS[141]
	{0x0F12, 0x01A1},	//#TVAR_ash_pGAS[142]
	{0x0F12, 0x0168},	//#TVAR_ash_pGAS[143]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[144]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[145]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[146]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[147]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[148]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[149]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[150]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[151]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[152]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[153]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[154]
	{0x0F12, 0x0163},	//#TVAR_ash_pGAS[155]
	{0x0F12, 0x0131},	//#TVAR_ash_pGAS[156]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[157]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[158]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[159]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[160]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[161]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[162]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[163]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[164]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[165]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[166]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[167]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[168]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[169]
	{0x0F12, 0x00D2},	//#TVAR_ash_pGAS[170]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[171]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[172]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[173]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[174]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[175]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[176]
	{0x0F12, 0x0059},	//#TVAR_ash_pGAS[177]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[178]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[179]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[180]
	{0x0F12, 0x010C},	//#TVAR_ash_pGAS[181]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[182]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[183]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[184]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[185]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[186]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[187]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[188]
	{0x0F12, 0x0025},	//#TVAR_ash_pGAS[189]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[190]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[191]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[192]
	{0x0F12, 0x00C2},	//#TVAR_ash_pGAS[193]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[194]
	{0x0F12, 0x00DD},	//#TVAR_ash_pGAS[195]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[196]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[197]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[198]
	{0x0F12, 0x001E},	//#TVAR_ash_pGAS[199]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[200]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[201]
	{0x0F12, 0x0010},	//#TVAR_ash_pGAS[202]
	{0x0F12, 0x0026},	//#TVAR_ash_pGAS[203]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[204]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[205]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[206]
	{0x0F12, 0x00EC},	//#TVAR_ash_pGAS[207]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[208]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[209]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[210]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[211]
	{0x0F12, 0x0017},	//#TVAR_ash_pGAS[212]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[213]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[214]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[215]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[216]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[217]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[218]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[219]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[220]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[221]
	{0x0F12, 0x009A},	//#TVAR_ash_pGAS[222]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[223]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[224]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[225]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[226]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[227]
	{0x0F12, 0x000F},	//#TVAR_ash_pGAS[228]
	{0x0F12, 0x0027},	//#TVAR_ash_pGAS[229]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[230]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[231]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[232]
	{0x0F12, 0x00F1},	//#TVAR_ash_pGAS[233]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[234]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[235]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[236]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[237]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[238]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[239]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[240]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[241]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[242]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[243]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[244]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[245]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[246]
	{0x0F12, 0x00FA},	//#TVAR_ash_pGAS[247]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[248]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[249]
	{0x0F12, 0x0064},	//#TVAR_ash_pGAS[250]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[251]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[252]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[253]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[254]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[255]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[256]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[257]
	{0x0F12, 0x00DF},	//#TVAR_ash_pGAS[258]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[259]
	{0x0F12, 0x0110},	//#TVAR_ash_pGAS[260]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[261]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[262]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[263]
	{0x0F12, 0x006E},	//#TVAR_ash_pGAS[264]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[265]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[266]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[267]
	{0x0F12, 0x0083},	//#TVAR_ash_pGAS[268]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[269]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[270]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[271]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[272]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[273]
	{0x0F12, 0x0104},	//#TVAR_ash_pGAS[274]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[275]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[276]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[277]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[278]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[279]
	{0x0F12, 0x0098},	//#TVAR_ash_pGAS[280]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[281]
	{0x0F12, 0x00D1},	//#TVAR_ash_pGAS[282]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[283]
	{0x0F12, 0x0127},	//#TVAR_ash_pGAS[284]
	{0x0F12, 0x0146},	//#TVAR_ash_pGAS[285]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[286]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[287]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[288]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[289]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[290]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[291]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[292]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[293]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[294]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[295]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[296]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[297]
	{0x0F12, 0x0180},	//#TVAR_ash_pGAS[298]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[299]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[300]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[301]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[302]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[303]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[304]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[305]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[306]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[307]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[308]
	{0x0F12, 0x00EE},	//#TVAR_ash_pGAS[309]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[310]
	{0x0F12, 0x014C},	//#TVAR_ash_pGAS[311]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[312]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[313]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[314]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[315]
	{0x0F12, 0x0057},	//#TVAR_ash_pGAS[316]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[317]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[318]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[319]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[320]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[321]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[322]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[323]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[324]
	{0x0F12, 0x00F6},	//#TVAR_ash_pGAS[325]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[326]
	{0x0F12, 0x0081},	//#TVAR_ash_pGAS[327]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[328]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[329]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[330]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[331]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[332]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[333]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[334]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[335]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[336]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[337]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[338]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[339]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[340]
	{0x0F12, 0x0041},	//#TVAR_ash_pGAS[341]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[342]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[343]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[344]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[345]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[346]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[347]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[348]
	{0x0F12, 0x00BA},	//#TVAR_ash_pGAS[349]
	{0x0F12, 0x00F7},	//#TVAR_ash_pGAS[350]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[351]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[352]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[353]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[354]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[355]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[356]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[357]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[358]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[359]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[360]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[361]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[362]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[363]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[364]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[365]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[366]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[367]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[368]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[369]
	{0x0F12, 0x0002},	//#TVAR_ash_pGAS[370]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[371]
	{0x0F12, 0x0023},	//#TVAR_ash_pGAS[372]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[373]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[374]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[375]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[376]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[377]
	{0x0F12, 0x00B0},	//#TVAR_ash_pGAS[378]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[379]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[380]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[381]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[382]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[383]
	{0x0F12, 0x001C},	//#TVAR_ash_pGAS[384]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[385]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[386]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[387]
	{0x0F12, 0x00B7},	//#TVAR_ash_pGAS[388]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[389]
	{0x0F12, 0x0106},	//#TVAR_ash_pGAS[390]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[391]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[392]
	{0x0F12, 0x006C},	//#TVAR_ash_pGAS[393]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[394]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[395]
	{0x0F12, 0x0033},	//#TVAR_ash_pGAS[396]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[397]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[398]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[399]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[400]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[401]
	{0x0F12, 0x0101},	//#TVAR_ash_pGAS[402]
	{0x0F12, 0x0120},	//#TVAR_ash_pGAS[403]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[404]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[405]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[406]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[407]
	{0x0F12, 0x0062},	//#TVAR_ash_pGAS[408]
	{0x0F12, 0x005C},	//#TVAR_ash_pGAS[409]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[410]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[411]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[412]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[413]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[414]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[415]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[416]
	{0x0F12, 0x0115},	//#TVAR_ash_pGAS[417]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[418]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[419]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[420]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[421]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[422]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[423]
	{0x0F12, 0x00A4},	//#TVAR_ash_pGAS[424]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[425]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[426]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[427]
	{0x0F12, 0x0130},	//#TVAR_ash_pGAS[428]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[429]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[430]
	{0x0F12, 0x00C6},	//#TVAR_ash_pGAS[431]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[432]
	{0x0F12, 0x0090},	//#TVAR_ash_pGAS[433]
	{0x0F12, 0x0085},	//#TVAR_ash_pGAS[434]
	{0x0F12, 0x0082},	//#TVAR_ash_pGAS[435]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[436]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[437]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[438]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[439]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[440]
	{0x0F12, 0x0135},	//#TVAR_ash_pGAS[441]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[442]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[443]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[444]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[445]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[446]
	{0x0F12, 0x005D},	//#TVAR_ash_pGAS[447]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[448]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[449]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[450]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[451]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[452]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[453]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[454]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[455]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[456]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[457]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[458]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[459]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[460]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[461]
	{0x0F12, 0x003D},	//#TVAR_ash_pGAS[462]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[463]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[464]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[465]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[466]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[467]
	{0x0F12, 0x00B5},	//#TVAR_ash_pGAS[468]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[469]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[470]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[471]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[472]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[473]
	{0x0F12, 0x001A},	//#TVAR_ash_pGAS[474]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[475]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[476]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[477]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[478]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[479]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[480]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[481]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[482]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[483]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[484]
	{0x0F12, 0x0018},	//#TVAR_ash_pGAS[485]
	{0x0F12, 0x000A},	//#TVAR_ash_pGAS[486]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[487]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[488]
	{0x0F12, 0x001F},	//#TVAR_ash_pGAS[489]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[490]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[491]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[492]
	{0x0F12, 0x00B6},	//#TVAR_ash_pGAS[493]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[494]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[495]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[496]
	{0x0F12, 0x0028},	//#TVAR_ash_pGAS[497]
	{0x0F12, 0x0012},	//#TVAR_ash_pGAS[498]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[499]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[500]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[501]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[502]
	{0x0F12, 0x0030},	//#TVAR_ash_pGAS[503]
	{0x0F12, 0x0050},	//#TVAR_ash_pGAS[504]
	{0x0F12, 0x007B},	//#TVAR_ash_pGAS[505]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[506]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[507]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[508]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[509]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[510]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[511]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[512]
	{0x0F12, 0x0001},	//#TVAR_ash_pGAS[513]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[514]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[515]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[516]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[517]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[518]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[519]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[520]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[521]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[522]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[523]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[524]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[525]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[526]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[527]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[528]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[529]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[530]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[531]
	{0x0F12, 0x00AE},	//#TVAR_ash_pGAS[532]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[533]
	{0x0F12, 0x0097},	//#TVAR_ash_pGAS[534]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[535]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[536]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[537]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[538]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[539]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[540]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[541]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[542]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[543]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[544]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[545]
	{0x0F12, 0x00D3},	//#TVAR_ash_pGAS[546]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[547]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[548]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[549]
	{0x0F12, 0x0058},	//#TVAR_ash_pGAS[550]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[551]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[552]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[553]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[554]
	{0x0F12, 0x006D},	//#TVAR_ash_pGAS[555]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[556]
	{0x0F12, 0x00AC},	//#TVAR_ash_pGAS[557]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[558]
	{0x0F12, 0x00F5},	//#TVAR_ash_pGAS[559]
	{0x0F12, 0x00D4},	//#TVAR_ash_pGAS[560]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[561]
	{0x0F12, 0x008E},	//#TVAR_ash_pGAS[562]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[563]
	{0x0F12, 0x0070},	//#TVAR_ash_pGAS[564]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[565]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[566]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[567]
	{0x0F12, 0x0093},	//#TVAR_ash_pGAS[568]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[569]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[570]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[571]



	//=================================================================================================
	//	Set Gamma LUT
	//=================================================================================================
	//							//New	//Hong	//ORG
	{0x002A, 0x04CC},
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[0][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[0][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[0][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[0][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[0][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[0][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[0][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[0][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[0][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[0][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[0][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[0][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[0][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[0][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[0][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[0][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[1][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[1][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[1][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[1][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[1][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[1][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[1][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[1][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[1][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[1][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[1][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[1][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[1][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[1][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[1][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[1][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[2][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[2][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[2][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[2][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[2][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[2][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[2][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[2][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[2][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[2][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[2][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[2][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[2][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[2][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[2][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[2][15]



	//=================================================================================================
	//	Set AWB
	//=================================================================================================
	{0x002A, 0x0DA8},
	{0x0F12, 0x0020},	// #awbb_LowBr0_NBzone	// 2008.04.24. eliminate to the purple color at low light.
	{0x002A, 0x0E8C},
	{0x0F12, 0x0000},	// #awbb_LowBr0_PatchNumZone
	{0x002A, 0x0D6C},
	{0x0F12, 0x0040},	// #awbb_YMedMoveToYAv

	// Indoor Gray Zone
	{0x002A, 0x0B9C},
	{0x0F12, 0x0339},	//0392	// #awbb_IndoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x0393},	//03A4	// #awbb_IndoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x031B},	//036F	// #awbb_IndoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x039C},	//03AE	// #awbb_IndoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x030A},	//034E	// #awbb_IndoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0398},	//03A2	// #awbb_IndoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x02F3},	//0331	// #awbb_IndoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0392},	//0392	// #awbb_IndoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x02E0},	//0315	// #awbb_IndoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0388},	//0381	// #awbb_IndoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x02CB},	//02F8	// #awbb_IndoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0374},	//036C	// #awbb_IndoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x02BB},	//02E1	// #awbb_IndoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x034B},	//0358	// #awbb_IndoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x02AA},	//02C5	// #awbb_IndoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x0336},	//033B	// #awbb_IndoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x029C},	//02B1	// #awbb_IndoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0321},	//0323	// #awbb_IndoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x028B},	//029C	// #awbb_IndoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x030B},	//030F	// #awbb_IndoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x027B},	//028B	// #awbb_IndoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02FD},	//02FA	// #awbb_IndoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x0268},	//027C	// #awbb_IndoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x02EA},	//02E2	// #awbb_IndoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x0259},	//0268	// #awbb_IndoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x02D8},	//02D1	// #awbb_IndoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x024C},	//0259	// #awbb_IndoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x02CB},	//02BF	// #awbb_IndoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x023D},	//024C	// #awbb_IndoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x02BD},	//02B1	// #awbb_IndoorGrZones_m_BGrid_14__m_right
	{0x0F12, 0x0231},	//023D	// #awbb_IndoorGrZones_m_BGrid_15__m_left
	{0x0F12, 0x02B1},	//02A2	// #awbb_IndoorGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0222},	//0231	// #awbb_IndoorGrZones_m_BGrid_16__m_left
	{0x0F12, 0x02A8},	//0299	// #awbb_IndoorGrZones_m_BGrid_16__m_right
	{0x0F12, 0x0215},	//0222	// #awbb_IndoorGrZones_m_BGrid_17__m_left
	{0x0F12, 0x029B},	//0290	// #awbb_IndoorGrZones_m_BGrid_17__m_right
	{0x0F12, 0x0208},	//0215	// #awbb_IndoorGrZones_m_BGrid_18__m_left
	{0x0F12, 0x0290},	//0288	// #awbb_IndoorGrZones_m_BGrid_18__m_right
	{0x0F12, 0x01F7},	//0208	// #awbb_IndoorGrZones_m_BGrid_19__m_left
	{0x0F12, 0x0287},	//0281	// #awbb_IndoorGrZones_m_BGrid_19__m_right
	{0x0F12, 0x01E6},	//01F7	// #awbb_IndoorGrZones_m_BGrid_20__m_left
	{0x0F12, 0x027F},	//0279	// #awbb_IndoorGrZones_m_BGrid_20__m_right
	{0x0F12, 0x01D9},	//01E6	// #awbb_IndoorGrZones_m_BGrid_21__m_left
	{0x0F12, 0x0279},	//026E	// #awbb_IndoorGrZones_m_BGrid_21__m_right
	{0x0F12, 0x01CC},	//01D9	// #awbb_IndoorGrZones_m_BGrid_22__m_left
	{0x0F12, 0x0272},	//0268	// #awbb_IndoorGrZones_m_BGrid_22__m_right
	{0x0F12, 0x01C1},	//01CC	// #awbb_IndoorGrZones_m_BGrid_23__m_left
	{0x0F12, 0x026C},	//0263	// #awbb_IndoorGrZones_m_BGrid_23__m_right
	{0x0F12, 0x01B3},	//01C1	// #awbb_IndoorGrZones_m_BGrid_24__m_left
	{0x0F12, 0x0265},	//0260	// #awbb_IndoorGrZones_m_BGrid_24__m_right
	{0x0F12, 0x01AA},	//01B3	// #awbb_IndoorGrZones_m_BGrid_25__m_left
	{0x0F12, 0x025C},	//0259	// #awbb_IndoorGrZones_m_BGrid_25__m_right
	{0x0F12, 0x019F},	//01AA	// #awbb_IndoorGrZones_m_BGrid_26__m_left
	{0x0F12, 0x0250},	//0253	// #awbb_IndoorGrZones_m_BGrid_26__m_right
	{0x0F12, 0x019B},	//019F	// #awbb_IndoorGrZones_m_BGrid_27__m_left
	{0x0F12, 0x0247},	//0248	// #awbb_IndoorGrZones_m_BGrid_27__m_right
	{0x0F12, 0x0197},	//019B	// #awbb_IndoorGrZones_m_BGrid_28__m_left
	{0x0F12, 0x0239},	//023F	// #awbb_IndoorGrZones_m_BGrid_28__m_right
	{0x0F12, 0x019B},	//0197	// #awbb_IndoorGrZones_m_BGrid_29__m_left
	{0x0F12, 0x021D},	//0239	// #awbb_IndoorGrZones_m_BGrid_29__m_right
	{0x0F12, 0x019D},	//019B	// #awbb_IndoorGrZones_m_BGrid_30__m_left
	{0x0F12, 0x0202},	//021D	// #awbb_IndoorGrZones_m_BGrid_30__m_right
	{0x0F12, 0x01AC},	//019D	// #awbb_IndoorGrZones_m_BGrid_31__m_left
	{0x0F12, 0x01D3},	//020A	// #awbb_IndoorGrZones_m_BGrid_31__m_right
	{0x0F12, 0x0000},	//01AC	// #awbb_IndoorGrZones_m_BGrid_32__m_left
	{0x0F12, 0x0000},	//01D3	// #awbb_IndoorGrZones_m_BGrid_32__m_right
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_left
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_right

	{0x002A, 0x0C3C},
	{0x0F12, 0x0004},	// #awbb_IndoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0138},	// #awbb_IndoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x0F12, 0x0000},

	// Outdoor Gray Zone
	{0x0F12, 0x0255},	//#awbb_OutdoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x0236},	//#awbb_OutdoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x0290},	//#awbb_OutdoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x0227},	//#awbb_OutdoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0297},	//#awbb_OutdoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x0210},	//#awbb_OutdoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0293},	//#awbb_OutdoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0200},	//#awbb_OutdoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x028B},	//#awbb_OutdoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x01F1},	//#awbb_OutdoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0286},	//#awbb_OutdoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x0282},	//#awbb_OutdoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x01E0},	//#awbb_OutdoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x027B},	//#awbb_OutdoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x01DE},	//#awbb_OutdoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0274},	//#awbb_OutdoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01DF},	//#awbb_OutdoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x0263},	//#awbb_OutdoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01F6},	//#awbb_OutdoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x0253},	//#awbb_OutdoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x020B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x023B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_right

	{0x002A, 0x0CB8},
	{0x0F12, 0x0004},	// #awbb_OutdoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x000C},	//#awbb_OutdoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0217},	//#awbb_OutdoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high
	{0x0F12, 0x0000},

	// Low Brightness Gray Zone
	{0x0F12, 0x0324},	//031E  // #awbb_LowBrGrZones_m_BGrid_0__m_left
	{0x0F12, 0x03A1},	//037E  // #awbb_LowBrGrZones_m_BGrid_0__m_right
	{0x0F12, 0x02F2},	//02CF  // #awbb_LowBrGrZones_m_BGrid_1__m_left
	{0x0F12, 0x03BA},	//0389  // #awbb_LowBrGrZones_m_BGrid_1__m_right
	{0x0F12, 0x02C4},	//02A6  // #awbb_LowBrGrZones_m_BGrid_2__m_left
	{0x0F12, 0x03B6},	//0386  // #awbb_LowBrGrZones_m_BGrid_2__m_right
	{0x0F12, 0x029E},	//0282  // #awbb_LowBrGrZones_m_BGrid_3__m_left
	{0x0F12, 0x03A6},	//0374  // #awbb_LowBrGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0283},	//0259  // #awbb_LowBrGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0380},	//0352  // #awbb_LowBrGrZones_m_BGrid_4__m_right
	{0x0F12, 0x025F},	//0239  // #awbb_LowBrGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0344},	//0330  // #awbb_LowBrGrZones_m_BGrid_5__m_right
	{0x0F12, 0x0241},	//021E  // #awbb_LowBrGrZones_m_BGrid_6__m_left
	{0x0F12, 0x031F},	//031B  // #awbb_LowBrGrZones_m_BGrid_6__m_right
	{0x0F12, 0x0227},	//020B  // #awbb_LowBrGrZones_m_BGrid_7__m_left
	{0x0F12, 0x02FC},	//0300  // #awbb_LowBrGrZones_m_BGrid_7__m_right
	{0x0F12, 0x020A},	//01F1  // #awbb_LowBrGrZones_m_BGrid_8__m_left
	{0x0F12, 0x02D9},	//02DD  // #awbb_LowBrGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01F1},	//01E0  // #awbb_LowBrGrZones_m_BGrid_9__m_left
	{0x0F12, 0x02C4},	//02C4  // #awbb_LowBrGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01D0},	//01CD  // #awbb_LowBrGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02B2},	//02AF  // #awbb_LowBrGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01B8},	//019E  // #awbb_LowBrGrZones_m_BGrid_11__m_left
	{0x0F12, 0x029E},	//02A0  // #awbb_LowBrGrZones_m_BGrid_11__m_right
	{0x0F12, 0x019F},	//0187  // #awbb_LowBrGrZones_m_BGrid_12__m_left
	{0x0F12, 0x0290},	//028C  // #awbb_LowBrGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0189},	//017D  // #awbb_LowBrGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0282},	//027E  // #awbb_LowBrGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0182},	//0179  // #awbb_LowBrGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0271},	//0258  // #awbb_LowBrGrZones_m_BGrid_14__m_right
	{0x0F12, 0x017F},	//0186  // #awbb_LowBrGrZones_m_BGrid_15__m_left
	{0x0F12, 0x0254},	//0232  // #awbb_LowBrGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0183},	//01BD  // #awbb_LowBrGrZones_m_BGrid_16__m_left
	{0x0F12, 0x022D},	//01FB  // #awbb_LowBrGrZones_m_BGrid_16__m_right
	{0x0F12, 0x019C},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_left
	{0x0F12, 0x01D3},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_right

	{0x002A, 0x0D34},
	{0x0F12, 0x0005},	// #awbb_LowBrGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0012},	// #awbb_LowBrGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0120},	// #awbb_LowBrGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0002},	// #awbb_LowBrGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D48},
	{0x0F12, 0x00E0},	// #awbb_LowBrGrZones_y_high
	{0x0F12, 0x0000},

	// Lowtemp Circle
	{0x0F12, 0x032F},	// #awbb_CrclLowT_R_c
	{0x0F12, 0x0000},
	{0x0F12, 0x017A},	// #awbb_CrclLowT_B_c
	{0x0F12, 0x0000},
	{0x0F12, 0x7300},	// #awbb_CrclLowT_Rad_c
	{0x0F12, 0x0000},
	{0x0F12, 0x000A},	// #awbb_CrclLowT_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D60},
	{0x0F12, 0x00E0},	// #awbb_CrclLowT_y_high
	{0x0F12, 0x0000},
	{0x002A, 0x0D82},
	{0x0F12, 0x0001},	// #awbb_ByPass_LowTempMode

	// White Locus
	{0x002A, 0x0D66},
	{0x0F12, 0x011F},	// #awbb_IntcR
	{0x0F12, 0x0132},	// #awbb_IntcB
	{0x002A, 0x0D74},
	{0x0F12, 0x052A},	// #awbb_MvEq_RBthresh

	// Gamut Thresholds
	{0x002A, 0x0DAE},
	{0x0F12, 0x0036},	// #awbb_GamutWidthThr2
	{0x0F12, 0x001C},	// #awbb_GamutHeightThr2
	{0x002A, 0x0DAA},
	{0x0F12, 0x071A},	// #awbb_GamutWidthThr1
	{0x0F12, 0x03A4},	// #awbb_GamutHeightThr1

	// SceneDetection Thresholds
	{0x002A, 0x0D92},
	{0x0F12, 0x0CE4},	//0A8C	// #awbb_SunnyBr
	{0x0F12, 0x0096},	// #awbb_Sunny_NBzone
	{0x002A, 0x0E86},
	{0x0F12, 0x0216},	// #awbb_OutdoorWP_r
	{0x0F12, 0x029F},	// #awbb_OutdoorWP_b
	{0x002A, 0x0D96},
	{0x0F12, 0x0580},	// #awbb_CloudyBr
	{0x0F12, 0x0096},	// #awbb_Cloudy_NBzone
	{0x002A, 0x0DB2},
	{0x0F12, 0x00DA},	// #awbb_CloudyRB
	{0x002A, 0x0D9A},
	{0x0F12, 0x000A},	// #awbb_Cloudy_BdivRzone
	{0x002A, 0x0DB4},
	{0x0F12, 0x0459},	// #awbb_LowTempRB
	{0x002A, 0x0DA4},
	{0x0F12, 0x000E},	// #awbb_LowTemp_RBzone
	{0x002A, 0x0D64},
	{0x0F12, 0x0040},	// #awbb_DarkBr
	{0x002A, 0x0DA6},
	{0x0F12, 0x001E},	// #awbb_LowBr_NBzone
	{0x002A, 0x0D9C},
	{0x0F12, 0x001B},	// #awbb_MacbethGamut_WidthZone
	{0x0F12, 0x000E},	// #awbb_MacbethGamut_HeightZone
	{0x0F12, 0x0008},	// #awbb_MacbethGamut_WidthZone2
	{0x0F12, 0x0004},	// #awbb_MacbethGamut_HeightZone2

	// AWB Debug.(Outdoor Pink)
	{0x002A, 0x0E30},
	{0x0F12, 0x0000},	//	#awbb_OutdoorFltrSz (outdoor WB를 moving average filtering)

	// UseInvalidOutdoor option
	{0x002A, 0x0D88},
	{0x0F12, 0x0001},	// #awbb_Use_InvalidOutDoor

	// AWB input Y-Filter setting
	{0x002A, 0x0C48},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x002A, 0x0CC4},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high

	{0x002A, 0x0DC2},
	{0x0F12, 0x0030},	// #awbb_GnCurPntImmunity
	{0x0F12, 0x00C8},	// #awbb_GnFarPntImmunity
	{0x0F12, 0x012C},	// #awbb_GnCurPntLongJump
	{0x0F12, 0x0258},	// #awbb_GainsMaxMove
	{0x0F12, 0x0003},	// #awbb_GnMinMatchToJump

	// RB Gain Offset
	//WRITE 70000E32	FFE0	//#awbb_RGainOff
	//WRITE 70000E34	FFe0	//#awbb_BGainOff


	//=================================================================================================
	//	Set CCM
	//=================================================================================================
	// CCM Start Address
	{0x002A, 0x06D0},
	{0x0F12, 0x2800},   //#TVAR_wbt_pBaseCcmsAddr[0]
	{0x0F12, 0x7000},
	{0x0F12, 0x2824},   //#TVAR_wbt_pBaseCcmsAddr[1]
	{0x0F12, 0x7000},
	{0x0F12, 0x2848},   //#TVAR_wbt_pBaseCcmsAddr[2]
	{0x0F12, 0x7000},
	{0x0F12, 0x286C},   //#TVAR_wbt_pBaseCcmsAddr[3]
	{0x0F12, 0x7000},
	{0x0F12, 0x2890},   //#TVAR_wbt_pBaseCcmsAddr[4]
	{0x0F12, 0x7000},
	{0x0F12, 0x28B4},   //#TVAR_wbt_pBaseCcmsAddr[5]
	{0x0F12, 0x7000},
	{0x002A, 0x06EC},
	{0x0F12, 0x28D8},   //#TVAR_wbt_pOutdoorCcm
	{0x0F12, 0x7000},

	{0x002A, 0x2800},
	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	// Outdoor CCM
	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEA4},
	{0x0F12, 0x020F},
	{0x0F12, 0xFF8F},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},



	//=================================================================================================
	//	Set NB
	//=================================================================================================
	{0x002A, 0x07EA},
	{0x0F12, 0x0000},	//afit_bUseNoiseInd 0 : NB 1: Noise Index

	// param_start	SARR_uNormBrInDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrInDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrInDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrInDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrInDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrInDoor[4]

	// param_start	SARR_uNormBrOutDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrOutDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrOutDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrOutDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrOutDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrOutDoor[4]



	//=================================================================================================
	//	Set AFIT
	//=================================================================================================
	// AFIT Start Address
	{0x002A, 0x0814},
	{0x0F12, 0x082C},	//#TVAR_afit_pBaseVals
	{0x0F12, 0x7000},	//#TVAR_afit_pBaseVals

	// AFIT Table	//0223
	{0x002A, 0x082C},
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0000},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0028},	//SHARP_BLUR
	{0x0F12, 0xFFE2},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0028},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                   bin: desparity low
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"                bin: desparity high
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x06FF},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x030A},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"               ]negati fine
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"                  ]low fine
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"                ]high fine
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"                   ]high low thres
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0005},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01AA},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0000},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x001E},	//SHARP_BLUR
	{0x0F12, 0xFFE2},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x000E},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                  bin: desparity low
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x08B7},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0308},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01A0},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x000E},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0089},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0202},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0202},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0206},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x803C},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0006},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0205},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0000},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8050},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0006},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0000},	//UVDenoise_iYLowThresh
	{0x0F12, 0x0000},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0105},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0200},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x805A},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[0]
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[1]
	{0x0F12, 0x0800},	// # afit_pConstBaseVals[2]
	{0x0F12, 0x0600},	// # afit_pConstBaseVals[3]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[4]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[5]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[6]
	{0x0F12, 0x0300},	// # afit_pConstBaseVals[7]
	{0x0F12, 0x0002},	// # afit_pConstBaseVals[8]
	{0x0F12, 0x0400},	// # afit_pConstBaseVals[9]
	{0x0F12, 0x0106},	// # afit_pConstBaseVals[10]
	{0x0F12, 0x0005},	// # afit_pConstBaseVals[11]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[12]
	{0x0F12, 0x0003},	// # afit_pConstBaseVals[13]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[14]
	{0x0F12, 0xFFD6},	// # afit_pConstBaseVals[15]
	{0x0F12, 0x53C1},	// # afit_pConstBaseVals[16]
	{0x0F12, 0xE1FE},	// # afit_pConstBaseVals[17]
	{0x0F12, 0x0001},	// # afit_pConstBaseVals[18]

	{0x002A, 0x01EA},
	{0x0F12, 0x0000},	//#REG_TC_UserSharpBlur
	{0x002A, 0x0E32},
	{0x0F12, 0xFFF0},	//#awbb_RGainOff
	{0x0F12, 0x0000},	//#awbb_BGainOff

	// For fast AE
	{0x002A, 0x0FAC},
	{0x0F12, 0x0001},	// AE Gain output [0]    default : 0x10
	{0x0F12, 0x0008},	// AE Gain output [1]    default : 0x20
	{0x0F12, 0x0015},	// AE Gain output [2]    default : 0x40

	// For fast AWB
	{0x002A, 0x0DCA},
	{0x0F12, 0x0001},	// awbb_GnMinMatchToJump = 0x02 (default:0x03)
	{0x0028, 0x7000},
	{0x002A, 0x0E98},
	{0x0F12, 0x0300},	//bp_uMaxBrightnessFactor 0180 -> 0300
	{0x002A, 0x0E9E},
	{0x0F12, 0x0300},	//bp_uMinBrightnessFactor 0180 -> 0300

	// Update Changed Registers
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},

	{0x0028, 0xD000},
	{0x002A, 0x1088},
	{0x0F12, 0x05D5},

	{0x0028, 0xD000},
	{0x002A, 0x1094},
	{0x0F12, 0x0010},

	{0x0028, 0xD000},
	{0x002A, 0x1082},
	{0x0F12, 0x03FF},

	{0x0028, 0xD000},
	{0x002A, 0x1084},
	{0x0F12, 0x03FF},

    {SEQUENCE_WAIT_MS, 0x0064},
    {SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_init_640x480[] =
{
	{0x0028, 0xD000},
	{0x002A, 0x1030},
	{0x0F12, 0x0000},
	{0x002A, 0x0014},
	{0x0F12, 0x0001},

	{SEQUENCE_WAIT_MS , 0x0014},// Delay 100 -> 20

	{0x0028, 0x7000},
	{0x002A, 0x2080},
	{0x0F12, 0xB510},	// 700020800
	{0x0F12, 0xF000},	// 700020822
	{0x0F12, 0xF8FC},	// 700020844
	{0x0F12, 0xBC10},	// 700020866
	{0x0F12, 0xBC08},	// 700020888
	{0x0F12, 0x4718},	// 7000208AA
	{0x0F12, 0xB5FE},	// 7000208CC
	{0x0F12, 0x0007},	// 7000208EE
	{0x0F12, 0x683C},	// 700020900
	{0x0F12, 0x687E},	// 700020922
	{0x0F12, 0x1DA5},	// 700020944
	{0x0F12, 0x88A0},	// 700020966
	{0x0F12, 0x2800},	// 700020988
	{0x0F12, 0xD00B},	// 7000209AA
	{0x0F12, 0x88A8},	// 7000209CC
	{0x0F12, 0x2800},	// 7000209EE
	{0x0F12, 0xD008},	// 700020A00
	{0x0F12, 0x8820},	// 700020A22
	{0x0F12, 0x8829},	// 700020A44
	{0x0F12, 0x4288},	// 700020A66
	{0x0F12, 0xD301},	// 700020A88
	{0x0F12, 0x1A40},	// 700020AAA
	{0x0F12, 0xE000},	// 700020ACC
	{0x0F12, 0x1A08},	// 700020AEE
	{0x0F12, 0x9001},	// 700020B00
	{0x0F12, 0xE001},	// 700020B22
	{0x0F12, 0x2019},	// 700020B44
	{0x0F12, 0x9001},	// 700020B66
	{0x0F12, 0x497B},	// 700020B88
	{0x0F12, 0x466B},	// 700020BAA
	{0x0F12, 0x8A48},	// 700020BCC
	{0x0F12, 0x8118},	// 700020BEE
	{0x0F12, 0x8A88},	// 700020C00
	{0x0F12, 0x8158},	// 700020C22
	{0x0F12, 0x4879},	// 700020C44
	{0x0F12, 0x8940},	// 700020C66
	{0x0F12, 0x0040},	// 700020C88
	{0x0F12, 0x2103},	// 700020CAA
	{0x0F12, 0xF000},	// 700020CCC
	{0x0F12, 0xF902},	// 700020CEE
	{0x0F12, 0x88A1},	// 700020D00
	{0x0F12, 0x4288},	// 700020D22
	{0x0F12, 0xD908},	// 700020D44
	{0x0F12, 0x8828},	// 700020D66
	{0x0F12, 0x8030},	// 700020D88
	{0x0F12, 0x8868},	// 700020DAA
	{0x0F12, 0x8070},	// 700020DCC
	{0x0F12, 0x88A8},	// 700020DEE
	{0x0F12, 0x6038},	// 700020E00
	{0x0F12, 0xBCFE},	// 700020E22
	{0x0F12, 0xBC08},	// 700020E44
	{0x0F12, 0x4718},	// 700020E66
	{0x0F12, 0x88A9},	// 700020E88
	{0x0F12, 0x4288},	// 700020EAA
	{0x0F12, 0xD906},	// 700020ECC
	{0x0F12, 0x8820},	// 700020EEE
	{0x0F12, 0x8030},	// 700020F00
	{0x0F12, 0x8860},	// 700020F22
	{0x0F12, 0x8070},	// 700020F44
	{0x0F12, 0x88A0},	// 700020F66
	{0x0F12, 0x6038},	// 700020F88
	{0x0F12, 0xE7F2},	// 700020FAA
	{0x0F12, 0x9801},	// 700020FCC
	{0x0F12, 0xA902},	// 700020FEE
	{0x0F12, 0xF000},	// 700021000
	{0x0F12, 0xF8EE},	// 700021022
	{0x0F12, 0x0033},	// 700021044
	{0x0F12, 0x0029},	// 700021066
	{0x0F12, 0x9A02},	// 700021088
	{0x0F12, 0x0020},	// 7000210AA
	{0x0F12, 0xF000},	// 7000210CC
	{0x0F12, 0xF8F0},	// 7000210EE
	{0x0F12, 0x6038},	// 700021100
	{0x0F12, 0xE7E6},	// 700021122
	{0x0F12, 0xB5F0},	// 700021144
	{0x0F12, 0xB08B},	// 700021166
	{0x0F12, 0x0006},	// 700021188
	{0x0F12, 0x2000},	// 7000211AA
	{0x0F12, 0x9004},	// 7000211CC
	{0x0F12, 0x6835},	// 7000211EE
	{0x0F12, 0x6874},	// 700021200
	{0x0F12, 0x68B0},	// 700021222
	{0x0F12, 0x900A},	// 700021244
	{0x0F12, 0x68F0},	// 700021266
	{0x0F12, 0x9009},	// 700021288
	{0x0F12, 0x4F60},	// 7000212AA
	{0x0F12, 0x8979},	// 7000212CC
	{0x0F12, 0x084A},	// 7000212EE
	{0x0F12, 0x88A8},	// 700021300
	{0x0F12, 0x88A3},	// 700021322
	{0x0F12, 0x4298},	// 700021344
	{0x0F12, 0xD300},	// 700021366
	{0x0F12, 0x0018},	// 700021388
	{0x0F12, 0xF000},	// 7000213AA
	{0x0F12, 0xF8E1},	// 7000213CC
	{0x0F12, 0x9007},	// 7000213EE
	{0x0F12, 0x0021},	// 700021400
	{0x0F12, 0x0028},	// 700021422
	{0x0F12, 0xAA04},	// 700021444
	{0x0F12, 0xF000},	// 700021466
	{0x0F12, 0xF8E3},	// 700021488
	{0x0F12, 0x9006},	// 7000214AA
	{0x0F12, 0x88A8},	// 7000214CC
	{0x0F12, 0x2800},	// 7000214EE
	{0x0F12, 0xD102},	// 700021500
	{0x0F12, 0x27FF},	// 700021522
	{0x0F12, 0x1C7F},	// 700021544
	{0x0F12, 0xE047},	// 700021566
	{0x0F12, 0x88A0},	// 700021588
	{0x0F12, 0x2800},	// 7000215AA
	{0x0F12, 0xD101},	// 7000215CC
	{0x0F12, 0x2700},	// 7000215EE
	{0x0F12, 0xE042},	// 700021600
	{0x0F12, 0x8820},	// 700021622
	{0x0F12, 0x466B},	// 700021644
	{0x0F12, 0x8198},	// 700021666
	{0x0F12, 0x8860},	// 700021688
	{0x0F12, 0x81D8},	// 7000216AA
	{0x0F12, 0x8828},	// 7000216CC
	{0x0F12, 0x8118},	// 7000216EE
	{0x0F12, 0x8868},	// 700021700
	{0x0F12, 0x8158},	// 700021722
	{0x0F12, 0xA802},	// 700021744
	{0x0F12, 0xC803},	// 700021766
	{0x0F12, 0xF000},	// 700021788
	{0x0F12, 0xF8D2},	// 7000217AA
	{0x0F12, 0x9008},	// 7000217CC
	{0x0F12, 0x8ABA},	// 7000217EE
	{0x0F12, 0x9808},	// 700021800
	{0x0F12, 0x466B},	// 700021822
	{0x0F12, 0x4342},	// 700021844
	{0x0F12, 0x9202},	// 700021866
	{0x0F12, 0x8820},	// 700021888
	{0x0F12, 0x8198},	// 7000218AA
	{0x0F12, 0x8860},	// 7000218CC
	{0x0F12, 0x81D8},	// 7000218EE
	{0x0F12, 0x980A},	// 700021900
	{0x0F12, 0x9903},	// 700021922
	{0x0F12, 0xF000},	// 700021944
	{0x0F12, 0xF8C4},	// 700021966
	{0x0F12, 0x9A02},	// 700021988
	{0x0F12, 0x17D1},	// 7000219AA
	{0x0F12, 0x0E09},	// 7000219CC
	{0x0F12, 0x1889},	// 7000219EE
	{0x0F12, 0x1209},	// 700021A00
	{0x0F12, 0x4288},	// 700021A22
	{0x0F12, 0xDD1F},	// 700021A44
	{0x0F12, 0x8820},	// 700021A66
	{0x0F12, 0x466B},	// 700021A88
	{0x0F12, 0x8198},	// 700021AAA
	{0x0F12, 0x8860},	// 700021ACC
	{0x0F12, 0x81D8},	// 700021AEE
	{0x0F12, 0x980A},	// 700021B00
	{0x0F12, 0x9903},	// 700021B22
	{0x0F12, 0xF000},	// 700021B44
	{0x0F12, 0xF8B4},	// 700021B66
	{0x0F12, 0x9001},	// 700021B88
	{0x0F12, 0x8828},	// 700021BAA
	{0x0F12, 0x466B},	// 700021BCC
	{0x0F12, 0x8118},	// 700021BEE
	{0x0F12, 0x8868},	// 700021C00
	{0x0F12, 0x8158},	// 700021C22
	{0x0F12, 0x980A},	// 700021C44
	{0x0F12, 0x9902},	// 700021C66
	{0x0F12, 0xF000},	// 700021C88
	{0x0F12, 0xF8AA},	// 700021CAA
	{0x0F12, 0x8AB9},	// 700021CCC
	{0x0F12, 0x9A08},	// 700021CEE
	{0x0F12, 0x4351},	// 700021D00
	{0x0F12, 0x17CA},	// 700021D22
	{0x0F12, 0x0E12},	// 700021D44
	{0x0F12, 0x1851},	// 700021D66
	{0x0F12, 0x120A},	// 700021D88
	{0x0F12, 0x9901},	// 700021DAA
	{0x0F12, 0xF000},	// 700021DCC
	{0x0F12, 0xF890},	// 700021DEE
	{0x0F12, 0x0407},	// 700021E00
	{0x0F12, 0x0C3F},	// 700021E22
	{0x0F12, 0xE000},	// 700021E44
	{0x0F12, 0x2700},	// 700021E66
	{0x0F12, 0x8820},	// 700021E88
	{0x0F12, 0x466B},	// 700021EAA
	{0x0F12, 0xAA05},	// 700021ECC
	{0x0F12, 0x8198},	// 700021EEE
	{0x0F12, 0x8860},	// 700021F00
	{0x0F12, 0x81D8},	// 700021F22
	{0x0F12, 0x8828},	// 700021F44
	{0x0F12, 0x8118},	// 700021F66
	{0x0F12, 0x8868},	// 700021F88
	{0x0F12, 0x8158},	// 700021FAA
	{0x0F12, 0xA802},	// 700021FCC
	{0x0F12, 0xC803},	// 700021FEE
	{0x0F12, 0x003B},	// 700022000
	{0x0F12, 0xF000},	// 700022022
	{0x0F12, 0xF895},	// 700022044
	{0x0F12, 0x88A1},	// 700022066
	{0x0F12, 0x88A8},	// 700022088
	{0x0F12, 0x003A},	// 7000220AA
	{0x0F12, 0xF000},	// 7000220CC
	{0x0F12, 0xF898},	// 7000220EE
	{0x0F12, 0x0004},	// 700022100
	{0x0F12, 0xA804},	// 700022122
	{0x0F12, 0xC803},	// 700022144
	{0x0F12, 0x9A09},	// 700022166
	{0x0F12, 0x9B07},	// 700022188
	{0x0F12, 0xF000},	// 7000221AA
	{0x0F12, 0xF889},	// 7000221CC
	{0x0F12, 0xA806},	// 7000221EE
	{0x0F12, 0xC805},	// 700022200
	{0x0F12, 0x0021},	// 700022222
	{0x0F12, 0xF000},	// 700022244
	{0x0F12, 0xF88C},	// 700022266
	{0x0F12, 0x6030},	// 700022288
	{0x0F12, 0xB00B},	// 7000222AA
	{0x0F12, 0xBCF0},	// 7000222CC
	{0x0F12, 0xBC08},	// 7000222EE
	{0x0F12, 0x4718},	// 700022300
	{0x0F12, 0xB510},	// 700022322
	{0x0F12, 0x6800},	// 700022344
	{0x0F12, 0x2805},	// 700022366
	{0x0F12, 0xD11E},	// 700022388
	{0x0F12, 0x481D},	// 7000223AA
	{0x0F12, 0x491D},	// 7000223CC
	{0x0F12, 0x8800},	// 7000223EE
	{0x0F12, 0x8348},	// 700022400
	{0x0F12, 0x2301},	// 700022422
	{0x0F12, 0x31A0},	// 700022444
	{0x0F12, 0x720B},	// 700022466
	{0x0F12, 0x2000},	// 700022488
	{0x0F12, 0x000A},	// 7000224AA
	{0x0F12, 0x3A60},	// 7000224CC
	{0x0F12, 0x7150},	// 7000224EE
	{0x0F12, 0x71CB},	// 700022500
	{0x0F12, 0x22FF},	// 700022522
	{0x0F12, 0x3980},	// 700022544
	{0x0F12, 0x75CA},	// 700022566
	{0x0F12, 0x764A},	// 700022588
	{0x0F12, 0x000A},	// 7000225AA
	{0x0F12, 0x3260},	// 7000225CC
	{0x0F12, 0x7210},	// 7000225EE
	{0x0F12, 0x7250},	// 700022600
	{0x0F12, 0x2008},	// 700022622
	{0x0F12, 0x7488},	// 700022644
	{0x0F12, 0x74C8},	// 700022666
	{0x0F12, 0x4813},	// 700022688
	{0x0F12, 0x217E},	// 7000226AA
	{0x0F12, 0x43C9},	// 7000226CC
	{0x0F12, 0x8101},	// 7000226EE
	{0x0F12, 0x2306},	// 700022700
	{0x0F12, 0x5EC0},	// 700022722
	{0x0F12, 0xF000},	// 700022744
	{0x0F12, 0xF86C},	// 700022766
	{0x0F12, 0xBC10},	// 700022788
	{0x0F12, 0xBC08},	// 7000227AA
	{0x0F12, 0x4718},	// 7000227CC
	{0x0F12, 0xB570},	// 7000227EE
	{0x0F12, 0x2200},	// 700022800
	{0x0F12, 0x490E},	// 700022822
	{0x0F12, 0x480E},	// 700022844
	{0x0F12, 0x2401},	// 700022866
	{0x0F12, 0xF000},	// 700022888
	{0x0F12, 0xF86A},	// 7000228AA
	{0x0F12, 0x0022},	// 7000228CC
	{0x0F12, 0x490D},	// 7000228EE
	{0x0F12, 0x480D},	// 700022900
	{0x0F12, 0x2502},	// 700022922
	{0x0F12, 0xF000},	// 700022944
	{0x0F12, 0xF864},	// 700022966
	{0x0F12, 0x490C},	// 700022988
	{0x0F12, 0x480D},	// 7000229AA
	{0x0F12, 0x002A},	// 7000229CC
	{0x0F12, 0xF000},	// 7000229EE
	{0x0F12, 0xF85F},	// 700022A00
	{0x0F12, 0xBC70},	// 700022A22
	{0x0F12, 0xBC08},	// 700022A44
	{0x0F12, 0x4718},	// 700022A66
	{0x0F12, 0x1A28},	// 700022A88
	{0x0F12, 0x7000},	// 700022AAA
	{0x0F12, 0x0D64},	// 700022ACC
	{0x0F12, 0x7000},	// 700022AEE
	{0x0F12, 0x2370},	// 700022B00
	{0x0F12, 0x7000},	// 700022B22
	{0x0F12, 0x1C14},	// 700022B44
	{0x0F12, 0x7000},	// 700022B66
	{0x0F12, 0x14FA},	// 700022B88
	{0x0F12, 0x7000},	// 700022BAA
	{0x0F12, 0x2115},	// 700022BCC
	{0x0F12, 0x7000},	// 700022BEE
	{0x0F12, 0x622F},	// 700022C00
	{0x0F12, 0x0000},	// 700022C22
	{0x0F12, 0x208D},	// 700022C44
	{0x0F12, 0x7000},	// 700022C66
	{0x0F12, 0x6445},	// 700022C88
	{0x0F12, 0x0000},	// 700022CAA
	{0x0F12, 0x2233},	// 700022CCC
	{0x0F12, 0x7000},	// 700022CEE
	{0x0F12, 0x3B5B},	// 700022D00
	{0x0F12, 0x0000},	// 700022D22
	{0x0F12, 0x4778},	// 700022D44
	{0x0F12, 0x46C0},	// 700022D66
	{0x0F12, 0xF004},	// 700022D88
	{0x0F12, 0xE51F},	// 700022DAA
	{0x0F12, 0xA464},	// 700022DCC
	{0x0F12, 0x0000},	// 700022DEE
	{0x0F12, 0x4778},	// 700022E00
	{0x0F12, 0x46C0},	// 700022E22
	{0x0F12, 0xC000},	// 700022E44
	{0x0F12, 0xE59F},	// 700022E66
	{0x0F12, 0xFF1C},	// 700022E88
	{0x0F12, 0xE12F},	// 700022EAA
	{0x0F12, 0x6009},	// 700022ECC
	{0x0F12, 0x0000},	// 700022EEE
	{0x0F12, 0x4778},	// 700022F00
	{0x0F12, 0x46C0},	// 700022F22
	{0x0F12, 0xC000},	// 700022F44
	{0x0F12, 0xE59F},	// 700022F66
	{0x0F12, 0xFF1C},	// 700022F88
	{0x0F12, 0xE12F},	// 700022FAA
	{0x0F12, 0x622F},	// 700022FCC
	{0x0F12, 0x0000},	// 700022FEE
	{0x0F12, 0x4778},	// 700023000
	{0x0F12, 0x46C0},	// 700023022
	{0x0F12, 0xC000},	// 700023044
	{0x0F12, 0xE59F},	// 700023066
	{0x0F12, 0xFF1C},	// 700023088
	{0x0F12, 0xE12F},	// 7000230AA
	{0x0F12, 0x5F49},	// 7000230CC
	{0x0F12, 0x0000},	// 7000230EE
	{0x0F12, 0x4778},	// 700023100
	{0x0F12, 0x46C0},	// 700023122
	{0x0F12, 0xC000},	// 700023144
	{0x0F12, 0xE59F},	// 700023166
	{0x0F12, 0xFF1C},	// 700023188
	{0x0F12, 0xE12F},	// 7000231AA
	{0x0F12, 0x5FC7},	// 7000231CC
	{0x0F12, 0x0000},	// 7000231EE
	{0x0F12, 0x4778},	// 700023200
	{0x0F12, 0x46C0},	// 700023222
	{0x0F12, 0xC000},	// 700023244
	{0x0F12, 0xE59F},	// 700023266
	{0x0F12, 0xFF1C},	// 700023288
	{0x0F12, 0xE12F},	// 7000232AA
	{0x0F12, 0x5457},	// 7000232CC
	{0x0F12, 0x0000},	// 7000232EE
	{0x0F12, 0x4778},	// 700023300
	{0x0F12, 0x46C0},	// 700023322
	{0x0F12, 0xC000},	// 700023344
	{0x0F12, 0xE59F},	// 700023366
	{0x0F12, 0xFF1C},	// 700023388
	{0x0F12, 0xE12F},	// 7000233AA
	{0x0F12, 0x5FA3},	// 7000233CC
	{0x0F12, 0x0000},	// 7000233EE
	{0x0F12, 0x4778},	// 700023400
	{0x0F12, 0x46C0},	// 700023422
	{0x0F12, 0xC000},	// 700023444
	{0x0F12, 0xE59F},	// 700023466
	{0x0F12, 0xFF1C},	// 700023488
	{0x0F12, 0xE12F},	// 7000234AA
	{0x0F12, 0x51F9},	// 7000234CC
	{0x0F12, 0x0000},	// 7000234EE
	{0x0F12, 0x4778},	// 700023500
	{0x0F12, 0x46C0},	// 700023522
	{0x0F12, 0xC000},	// 700023544
	{0x0F12, 0xE59F},	// 700023566
	{0x0F12, 0xFF1C},	// 700023588
	{0x0F12, 0xE12F},	// 7000235AA
	{0x0F12, 0x7633},	// 7000235CC
	{0x0F12, 0x0000},	// 7000235EE
	{0x0F12, 0x4778},	// 700023600
	{0x0F12, 0x46C0},	// 700023622
	{0x0F12, 0xC000},	// 700023644
	{0x0F12, 0xE59F},	// 700023666
	{0x0F12, 0xFF1C},	// 700023688
	{0x0F12, 0xE12F},	// 7000236AA
	{0x0F12, 0xA007},	// 7000236CC
	{0x0F12, 0x0000},	// 7000236EE
	{0x0F12, 0x0360},	// 700023700
	{0x0F12, 0x6F4E},	// 700023722
	{0x0F12, 0x2076},	// 700023744
	{0x0F12, 0x3831},	// 700023766
	{0x0F12, 0x3220},	// 700023788
	{0x0F12, 0x3030},	// 7000237AA
	{0x0F12, 0x0038},	// 7000237CC
	{0x0F12, 0xE010},	// 7000237EE
	{0x0F12, 0x0208},	// 700023800
	{0x0F12, 0x0038},	// 700023822
	// End of Trap and Patch (Last : 70002382h) // Total Size 772 (0x0304)
	{0x0028, 0xD000},
	{0x002A, 0x1000},
	{0x0F12, 0x0001},

	{0x0028, 0x7000},
	{0x002A, 0x0ADC},
	{0x0F12, 0x0AF0},	//#setot_uOnlineClocksDiv4OnlineC0             //2800
	{0x002A, 0x0AE2},
	{0x0F12, 0x222E},	//#setot_usSetRomWaitStatesSetRomThreshold4KHz //ROM Wait Threshold to 35MHz
	{0x002A, 0x0B94},
	{0x0F12, 0x0500},	//#awbb_GainsInit_0_:R    insInit
	{0x0F12, 0x0400},	//#awbb_GainsInit_1_:G    insInit
	{0x0F12, 0x0700},	//#awbb_GainsInit_2_:B    insInit
	{0x002A, 0x04A0},
	{0x0F12, 0x8000},	//#lt_uLeiInit:AE start   Init:AE
	{0x002A, 0x049A},
	{0x0F12, 0x003B},	//#lt_uMinExp             Exp



	//=================================================================================================
	//	Set CIS/APS/Analog
	//=================================================================================================
	// This registers are for FACTORY ONLY. If you change it without prior notification,
	// YOU are RESPONSIBLE for the FAILURE that will happen in the future.
	//=================================================================================================
	{0x0028, 0xD000},
	{0x002A, 0xF106},
	{0x0F12, 0x0001},	// L-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xF206},
	{0x0F12, 0x0001},	// F-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xC202},
	{0x0F12, 0x0700},	// tgr_coarse_integration_time[15:0]: revised by Ana 080115
	{0x002A, 0xF414},
	{0x0F12, 0x0030},	// aig_adc_sat[7:0] (aig_adc_sat[7:4]+1)*33mV + 690mV
	{0x002A, 0xC204},
	{0x0F12, 0x0100},	// tgr_analogue_code_global[12:0] Analog gain X8
	{0x002A, 0xF402},
	{0x0F12, 0x0092},	// aig_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_offset2[7:0]: revised by Ana 080425
	{0x002A, 0xF700},
	{0x0F12, 0x0040},	// bpradlc_control[7:0]: revised by Ana 080529 // bpradlc_nactive_pedestal[7:5],bpradlc_passthrough[1],bpradlc_bypass[0]
	{0x002A, 0xF708},
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_r[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gr[7:0]: revised by Tg 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gb[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_b[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_total[7:0]: revised by TG 080529
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_r[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gr[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gb[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_b[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_total[7:0]: revised by Ana 080425
	{0x0F12, 0x0001},	// bpradlc_adlc_en[7:0]: revised by TG 080529	// bpradlc_f_l_adlc_en[2],bpradlc_f_adlc_en[1],bpradlc_h_adlc_en[0]
	{0x0F12, 0x0015},	// bpradlc_adlc_option[7:0]: revised by TG 080425 // bpradlc_h_adlc_ch_sel[4], // bpradlc_max_data_clip_sel[3:2],bpradlc_f_adlc_max_data_clip_sel[1:0]
	{0x0F12, 0x0001},	// bpradlc_adlc_bpr_en[7:0] adlc_bpr_enable: revised by Ana 080112
	{0x0F12, 0x0040},	// bpradlc_adlc_bpr_thresh[7:0]: 080425
	{0x002A, 0xF48A},
	{0x0F12, 0x0048},	// aig_ld_ptr7[9:0] line OB start addr(04h~48h): revised by Ana 080911
	{0x002A, 0xF10A},
	{0x0F12, 0x008B},	// tgr_h_desc0_addr_end[10:0] line OB end addr(47h~8Bh): revised by Ana 080911 // line OB start - line OB end = 67d
	{0x002A, 0xF900},
	{0x0F12, 0x0067},	// cfpn_config: revised by Ana 080425 // cfpn_ref_gain[9:7],cpfn_ref_lines[6:5],cfpn_hbinning[4],cfpn_output_direct[3],// cfpn_overflow_protect[2],cpfn_passthrough[1],cfpn_bypass[0]
	{0x002A, 0xF406},
	{0x0F12, 0x0092},	// aig_cfpn_ref_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_cfpn_ref_offset2[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_cfpn_ref_gain[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_ld_ctrl[1:0] aig_ldb_en[1], aig_ld_en[0]
	{0x0F12, 0x0000},	// aig_row_id_ctrl[1:0]
	{0x002A, 0xF442},
	{0x0F12, 0x0000},	// aig_vavg[0]
	{0x0F12, 0x0000},	// aig_havg[0]=1b @H-avg mode: revised by Ana 080116
	{0x002A, 0xF448},
	{0x0F12, 0x0000},	// aig_sl_off[0]
	{0x002A, 0xF454},
	{0x0F12, 0x0001},	//Power save enable
	{0x0F12, 0x0001},	// aig_blst_en[0]
	{0x0F12, 0x0010},	// aig_blst_en_cintr[15:0]
	{0x0F12, 0x0000},	// aig_dshut_en[0]
	{0x002A, 0xF41A},
	{0x0F12, 0x00FF},	// aig_comp_bias[7:0] aig_comp2_bias[7:4], aig_comp1_bias[3:0]: revised by Ana 081013
	{0x0F12, 0x0003},	// aig_pix_bias[3:0]
	{0x002A, 0xF420},
	{0x0F12, 0x0030},	// aig_clp_lvl[7:0]: revised by Ana 080910(refer to 6AA)
	{0x002A, 0xF410},
	{0x0F12, 0x0001},	// aig_clp_sl_ctrl[0]
	{0x0F12, 0x0000},	// aig_cds_test[7:0], aig_cds_test[1]=1b @H-avg mode current save: revised by Ana 080116
	{0x002A, 0xF416},
	{0x0F12, 0x0001},	// aig_rmp_option[7:0]
	{0x002A, 0xF424},
	{0x0F12, 0x0000},	// aig_ref_option[7:0], aig_ref_option[0]=1b @H-avg mode odd cds off: revised by Ana 080116
	{0x002A, 0xF422},
	{0x0F12, 0x0000},	// aig_monit[7:0]
	{0x002A, 0xF41E},
	{0x0F12, 0x0000},	// aig_pd_pix[0]
	{0x002A, 0xF428},
	{0x0F12, 0x0000},	// aig_pd_cp[1:0] aig_pd_ncp[1], aig_pd_cp[0]
	{0x0F12, 0x0000},	// aig_pd_reg_pix[0]
	{0x0F12, 0x0000},	// aig_pd_reg_rg[0]
	{0x002A, 0xF430},
	{0x0F12, 0x0000},	// aig_pd_reg_tgsl[0]
	{0x0F12, 0x0000},	// aig_pd_reg_ntg[0]
	{0x0F12, 0x0008},	// aig_rosc_tune_cp[3:0]: revised by Ana 080418
	{0x0F12, 0x0005},	// aig_rosc_tune_ncp[3:0]
	{0x0F12, 0x000F},	// aig_cp_capa[3:0]
	{0x0F12, 0x0001},	// aig_reg_tune_pix[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_rg[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_tgsl[7:0]
	{0x0F12, 0x0010},	// aig_reg_tune_ntg[7:0] 0040 -> 0010
	{0x002A, 0xF4D6},
	{0x0F12, 0x00C0},	// aig_v15_tune[7:0]: revised by Ana 081010 // aig_v15_tune[7:4]=7h @sensor only Mode // aig_v15_tune[7:4]=9h @ISP 7.5fps Mode // aig_v15_tune[7:4]=Dh @ISP 15fps Mode
	{0x0F12, 0x0000},	//aig_vreg_sel[7:0] [9]h_test, [1]reg_sw_stby, [0]aig_reg_sel
	{0x002A, 0xF47C},
	{0x0F12, 0x000C},	//aig_ld_ptr0[4:0]
	{0x0F12, 0x0000},	//aig_ld_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF49A},
	{0x0F12, 0x0008},	//aig_sla_ptr0[3:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_sla_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4A2},
	{0x0F12, 0x0008},	//aig_slb_ptr0[7:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_slb_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4B2},
	{0x0F12, 0x0013},	//aig_rxa_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxa_ptr1[9:0]: revised by Ana 081010
	{0x0F12, 0x0013},	//aig_rxb_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxb_ptr1[9:0]: revised by Ana 081010
	{0x002A, 0xF4AA},
	{0x0F12, 0x009B},	//aig_txa_ptr0[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txa_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_txb_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txb_ptr1[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF474},
	{0x0F12, 0x0017},	//aig_s3_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x005F},	//aig_s3_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0017},	//aig_s4_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x008F},	//aig_s4_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF48C},
	{0x0F12, 0x0017},	//aig_clp_sl_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_clp_sl_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF4C8},
	{0x0F12, 0x0163},	//aig_off_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0193},	//aig_rmp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF490},
	{0x0F12, 0x0191},	//aig_comp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF418},
	{0x0F12, 0x0083},	//aig_dbs_option[7:0]: revised by Ana 081010

	//For ESD Check
	{0x0028, 0x7000},
	{0x002A, 0x0132},
	{0x0F12, 0xAAAA},	//#define REG_FWpid (For ESD Check)
	// * note : ESD Check Method.
	//	1. if (7000.0132 != AAAA ) reset // ( S/W reg. check)
	//	2. if (D000.1002 != 0000 ) reset // (internal Exception)

	//Set FPN Gain Input
	{0x002A, 0x1176},
	{0x0F12, 0x0020},	// fpn_GainInput[0]
	{0x0F12, 0x0040},	// fpn_GainInput[1]
	{0x0F12, 0x0080},	// fpn_GainInput[2]
	{0x0F12, 0x0100},	// fpn_GainInput[3]
	{0x0F12, 0x0014},	// fpn_GainOutput[0]
	{0x0F12, 0x000A},	// fpn_GainOutput[1]
	{0x0F12, 0x0008},	// fpn_GainOutput[2]
	{0x0F12, 0x0004},	// fpn_GainOutput[3]

	// Set CFPN Canceller
	{0x002A, 0x116E},
	{0x0F12, 0x0000},	// OvflProtect
	{0x0F12, 0x0000},	// bypassThrough
	{0x0F12, 0x0000},	// bypass
	{0x0F12, 0x0002},	// fpn.CollectMethod	// 0 : Center	1 : Right	2: LEFT
	{0x002A, 0x0AE8},
	{0x0F12, 0x0000},	// setot_bSendFpnToISP = ??

	// Set Forbidden Area
	{0x002A, 0x10EE},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_usRegCount // 0 value --> not use aig table 3
	{0x002A, 0x10F2},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_pSenModesRegsArray3[0]		//real register 0xD000F45A
	{0x002A, 0x1152},
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[48]    //real register 0xD000F4BA
	{0x0F12, 0x0028},	// senHal_SenRegsModes3_pSenModesRegsArray3[49]    //real register 0xD000F4BC
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[50]    //real register 0xD000F4BE
	{0x002A, 0x1148},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[43]    //real register 0xD000F4B0
	{0x002A, 0x1144},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[41]    //real register 0xD000F4AC
	{0x002A, 0x1150},
	{0x0F12, 0x0148},	// senHal_SenRegsModes3_pSenModesRegsArray3[47]    //real register 0xD000F4B8

	// Set AE Target
	{0x002A, 0x0F4C},
	{0x0F12, 0x003D},	// #TVAR_ae_BrAve



	//=================================================================================================
	//	Set Frame Rate
	//=================================================================================================
	{0x002A, 0x0484},
	{0x0F12, 0x61A8},	// #lt_uMaxExp1 	// 50ms
	{0x0F12, 0x0000},
	{0x0F12, 0xE848},	// #lt_uMaxExp2 	// 250ms
	{0x0F12, 0x0001},
	{0x0F12, 0x03C4},	// #lt_uCapMaxExp1	// 133ms
	{0x0F12, 0x0001},
	{0x0F12, 0xE848},	// #lt_uCapMaxExp2	// 250ms
	{0x0F12, 0x0001},
	{0x0F12, 0x04C0},	// #lt_uMaxAnGain1	// x4.75
	{0x0F12, 0x0700},	// #lt_uMaxAnGain2	// x7.0
	{0x0F12, 0x0100},	// #lt_uMaxDigGain
	{0x002A, 0x0F52},
	{0x0F12, 0x0007},	// #ae_StatMode



	//=================================================================================================
	//	Set Flicker  Current Flicker = 7000.164A //Mon_AFC_usFlExpQunat		0001:Off 1045:60Hz	1388:50Hz
	//=================================================================================================

	//1. Auto Flicker 60Hz Start
	{0x002A, 0x0B2E},
	{0x0F12, 0x0001},	// #AFC_Default60Hz 	1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	{0x002A, 0x03F8},
	{0x0F12, 0x007F},	// #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////2. Auto Flicker 50Hz Start
	//WRITE	70000B2E	0000	// #AFC_Default60Hz		1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	//WRITE 700003F8	007F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////3. Manual Flicker 60Hz


	//=================================================================================================
	//	Set PLL
	//=================================================================================================
	// Input CLK
	{0x002A, 0x01B8},
	{0x0F12, 0x5DC0},	//5DC0	//#REG_TC_IPRM_InClockLSBs	//24 -> 24.5M Hz(20081208)
	{0x0F12, 0x0000},	//#REG_TC_IPRM_InClockMSBs

	// Target Sys CLK
	{0x002A, 0x01CC},
	{0x0F12, 0x1770},	// #REG_TC_IPRM_OpClk4KHz_0 			//24Mhz

	// Target PVI CLK
	{0x0F12, 0x2318},	// #REG_TC_IPRM_MinOutRate4KHz_0	//36Mhz
	{0x0F12, 0x2338},	// #REG_TC_IPRM_MaxOutRate4KHz_0

	// PLL ETC
	{0x002A, 0x01C6},
	{0x0F12, 0x0003},	//#REG_TC_IPRM_UseNPviClocks
	{0x0F12, 0x0000},	//#REG_TC_IPRM_UseNMipiClocks
	{0x002A, 0x01C0},
	{0x0F12, 0x0000},	//#EG_TC_IPRM_ValidVActiveLow
	{0x002A, 0x01DE},
	{0x0F12, 0x0001},	//#REG_TC_IPRM_UseRegsAPI
	{0x0F12, 0x0001},	//#REG_TC_IPRM_InitParamsUpdated
	{0x002A, 0x1084},
	{0x0F12, 0x0001},	//#senHal_bUseAnalogBinning //Avg S.S
	{0x002A, 0x10B6},
	{0x0F12, 0x0048},	//#senHal_uMinColsAddAnalogBin



	//=================================================================================================
	//	Set Preview Config
	//=================================================================================================

	//* Preview Cropping Code *//
#ifdef VC_CAMERA_CROP
	{0x002A,	0x020A},
//	{0x0F12,	0x0640},	//#REG_TC_GP_PrevZoomReqInputWidth
//	{0x0F12,	0x0384},	//#REG_TC_GP_PrevZoomReqInputHeight
	{0x0F12,	0x0500},	//#REG_TC_GP_PrevZoomReqInputWidthOfs		0x500(1280)
	{0x0F12,	0x03C0},	//#REG_TC_GP_PrevZoomReqInputHeightOfs		0x3C0(960)

	//* Zoom Update *//
	{0x0F12,	0x0000},	//#REG_TC_GP_PrevZoomReqInputWidthOffset
	{0x0F12,	0x00F0},	//#REG_TC_GP_PrevZoomReqInputHeightOffset

	{0x002A,	0x021A},
	{0x0F12,	0x0001},	// #REG_TC_GP_Inputs_ChangeRequest : 0x021A
#endif
	{0x002A, 0x0242},
	{0x0F12, 0x0280}, //0x0280, //0x0140(320)->640, //090416 800 -> 320, 0x0320,	                 1280 -> 0x500  //0280  // #REG_0TC_PCFG_usWidth						// 640 -> 800 _20081209
	{0x0F12, 0x01E0}, //0x01e0, //0x00f0(240)->480, //090416 600 -> 240, 0x0258, 0x168 -> 360     720 -> 0x2D0	//01e0  // #REG_0TC_PCFG_usHeight						// 480 -> 600 _20081209
	{0x0F12, 0x0005},	// #REG_0TC_PCFG_Format 						// YUV output
	{0x0F12, 0x2338},	// #REG_0TC_PCFG_usMaxOut4KHzRate
	{0x0F12, 0x2318},	// #REG_0TC_PCFG_usMinOut4KHzRate
//	{0x0F12, 0x446C},	// #REG_0TC_PCFG_usMaxOut4KHzRate
//	{0x0F12, 0x444C},	// #REG_0TC_PCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	// #REG_0TC_PCFG_PVIMask   (x2: PCLK rising x0: PCLK Falling)
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_uClockInd
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_usFrTimeType
//	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType		(0 : bin mode, 2 : No-bin mode)
	{0x0F12, 0x02AF},	//03E8},	// #REG_0TC_PCFG_usMaxFrTimeMsecMult10 // 10fps --> 15fps
	{0x0F12, 0x01F4},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 20fps
//	{0x0F12, 0x014D},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 30fps
	{0x002A, 0x0262},
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uPrevMirror -- X/Y flip 20090112
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uCaptureMirror -- X/Y flip 20090112
	//WRITE 70000262 000F  //#REG_0TC_PCFG_uPrevMirror
	//WRITE 70000264 000F  //#REG_0TC_PCFG_uCaptureMirror


	// Apply preview config
	{0x002A, 0x021C},
	{0x0F12, 0x0000},	//#REG_TC_GP_ActivePrevConfig	  //preview config0
	{0x002A, 0x0220},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevOpenAfterChange  //config 변경적용
	{0x002A, 0x01F8},
	{0x0F12, 0x0001},	//#REG_TC_GP_NewConfigSync
	{0x002A, 0x021E},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevConfigChanged



	//=================================================================================================
	//	Set Capture Config
	//=================================================================================================
	//WRITE 70000214	04C0	// #REG_TC_GP_CapZoomReqInputHeight	// Vsize = 1216
	//WRITE 70000216	0000	// #REG_TC_GP_CapZoomReqInputWidthOfs
	//WRITE 70000218	0000	// #REG_TC_GP_CapZoomReqInputHeightOfs
	//WRITE 7000021A	0001	// #REG_TC_GP_InputsChangeRequest
	//WRITE 700001F8	0001	// #REG_TC_GP_NewConfigSync
	//
#ifdef VC_CAMERA_CROP
	//* Capture Cropping Code *//
	{0x0028, 0x7000},
	{0x002A, 0x0212},
	{0x0F12, 0x0500}, //#REG_TC_GP_CapZoomReqInputWidth
	{0x0F12, 0x03C0}, //#REG_TC_GP_CapZoomReqInputHeight
	{0x0F12, 0x0000}, //#REG_TC_GP_CapZoomReqInputWidthOfs
	{0x0F12, 0x00f0}, //#REG_TC_GP_CapZoomReqInputHeightOfs
#endif
	{0x002A, 0x0302},
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uCaptureMode
	{0x0F12, 0x0500},	//#REG_0TC_CCFG_usWidth
	{0x0F12, 0x03C0},	//#REG_0TC_CCFG_usHeight						// Vsize = 1216
	{0x0F12, 0x0005},	//#REG_0TC_CCFG_Format	   //5:YUV,9:JPEG
	{0x0F12, 0x2338},	//#REG_0TC_CCFG_usMaxOut4KHzRate
	{0x0F12, 0x2318},	//#REG_0TC_CCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	//#REG_0TC_CCFG_PVIMask (x2: PCLK rising	x0: PCLK Falling)
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uClockInd
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_usFrTimeType
	{0x0F12, 0x0002},	//#REG_0TC_CCFG_FrRateQualityType     (0 : bin mode, 2 : No-bin mode)
	{0x0F12, 0x09C4},	//07D0},	//#REG_0TC_CCFG_usMaxFrTimeMsecMult10 // 5fps --> 4fps
	{0x0F12, 0x0535},	//#REG_0TC_CCFG_usMinFrTimeMsecMult10 // 7.5fps


	//* Capture Configuration Update *//
	{0x002A, 0x01F8},
	{0x0F12, 0x0001}, //REG_TC_GP_NewConfigSync 0001 : update configuration
	{0x002a, 0x0226},
	{0x0f12, 0x0001}, //REG_TC_GP_CapConfigChanged 0001

	//=================================================================================================
	//	Set Periodic Mismatch (Use default value)
	//=================================================================================================



	//=================================================================================================
	//	Set AE Weights
	//=================================================================================================
	{0x002A, 0x0F5A},
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_0_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_1_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_2_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_3_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_4_
	{0x0F12, 0x0000},	//0201	// #ae_WeightTbl_16_5_
	{0x0F12, 0x0000},	//0102	// #ae_WeightTbl_16_6_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_7_
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_8_
	{0x0F12, 0x0101},	//0202	// #ae_WeightTbl_16_9_
	{0x0F12, 0x0101},	//0202	// #ae_WeightTbl_16_10
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_11
	{0x0F12, 0x0201},	//0101	// #ae_WeightTbl_16_12
	{0x0F12, 0x0202},	//0402	// #ae_WeightTbl_16_13
	{0x0F12, 0x0102},	//0204	// #ae_WeightTbl_16_14
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_15
	{0x0F12, 0x0201},	//0201	// #ae_WeightTbl_16_16
	{0x0F12, 0x0505},	//0504	// #ae_WeightTbl_16_17
	{0x0F12, 0x0102},	//0405	// #ae_WeightTbl_16_18
	{0x0F12, 0x0000},	//0102	// #ae_WeightTbl_16_19
	{0x0F12, 0x0201},	//0501	// #ae_WeightTbl_16_20
	{0x0F12, 0x0505},	//0505	// #ae_WeightTbl_16_21
	{0x0F12, 0x0102},	//0505	// #ae_WeightTbl_16_22
	{0x0F12, 0x0000},	//0105	// #ae_WeightTbl_16_23
	{0x0F12, 0x0201},	//0503	// #ae_WeightTbl_16_24
	{0x0F12, 0x0202},	//0505	// #ae_WeightTbl_16_25
	{0x0F12, 0x0102},	//0505	// #ae_WeightTbl_16_26
	{0x0F12, 0x0000},	//0305	// #ae_WeightTbl_16_27
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_28
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_29
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_30
	{0x0F12, 0x0000},	//0303	// #ae_WeightTbl_16_31



	//=================================================================================================
	//	Set GAS & CCM White Point
	//=================================================================================================
	// param_start	TVAR_ash_AwbAshCord
	{0x002A, 0x0704},
	{0x0F12, 0x0090},	//000E	// #TVAR_ash_AwbAshCord_0_
	{0x0F12, 0x00B3},	//0012	// #TVAR_ash_AwbAshCord_1_
	{0x0F12, 0x00C3},	//0019	// #TVAR_ash_AwbAshCord_2_
	{0x0F12, 0x00E5},	//0034	// #TVAR_ash_AwbAshCord_3_
	{0x0F12, 0x0116},	//0044	// #TVAR_ash_AwbAshCord_4_
	{0x0F12, 0x0198},	//0052	// #TVAR_ash_AwbAshCord_5_
	{0x0F12, 0x0200},	//0058	// #TVAR_ash_AwbAshCord_6_

	// param_start	wbt_AwbCcmCord
	{0x002A, 0x06F2},
	{0x0F12, 0x00B3},	//0016	// #SARR_AwbCcmCord_0_	Hor
	{0x0F12, 0x00E5},	//001E	// #SARR_AwbCcmCord_1_	IncaA
	{0x0F12, 0x0100},	//0024	// #SARR_AwbCcmCord_2_	WW
	{0x0F12, 0x0116},	//0038	// #SARR_AwbCcmCord_3_	CW
	{0x0F12, 0x0150},	//004C	// #SARR_AwbCcmCord_4_	D50
	{0x0F12, 0x0198},	//0058	// #SARR_AwbCcmCord_5_	D65

	// Set Target Brightness Control
	{0x002A, 0x103E},
	{0x0F12, 0x0000},	// #SARR_IllumType_0_
	{0x0F12, 0x0009},	// #SARR_IllumType_1_
	{0x0F12, 0x0018},	// #SARR_IllumType_2_
	{0x0F12, 0x0032},	// #SARR_IllumType_3_
	{0x0F12, 0x004A},	// #SARR_IllumType_4_
	{0x0F12, 0x0051},	// #SARR_IllumType_5_
	{0x0F12, 0x0056},	// #SARR_IllumType_6_
	{0x0F12, 0x010C},	// #SARe_2_R_IllumTypeF_0_
	{0x0F12, 0x010C},	// #SARe_3_R_IllumTypeF_1_
	{0x0F12, 0x0109},	// #SARe_4_R_IllumTypeF_2_
	{0x0F12, 0x0105},	// #SARe_5_R_IllumTypeF_3_
	{0x0F12, 0x0102},	// #SARe_6_R_IllumTypeF_4_
	{0x0F12, 0x00FB},	// #SARR_IllumTypeF_5_
	{0x0F12, 0x00F8},	// #SARR_IllumTypeF_6_

	// Indoor GAS Alpha
	{0x002A, 0x0712},
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[0]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[1]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[2]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[3]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[4]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[5]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[6]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[7]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[8]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[9]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[10]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[11]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[12]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[13]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[14]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[15]
	{0x0F12, 0x00E0},	// #TVAR_ash_GASAlpha[16]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[17]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[18]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[19]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[20]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[21]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[22]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[23]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[24]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[25]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[26]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[27]

	// Outdoor GAS Alpha
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_0_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_1_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_2_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_3_

	// GAS LUT Start Address
	{0x002A, 0x0754},
	{0x0F12, 0x2388},	//#TVAR_ash_pGAS
	{0x0F12, 0x7000},	//#TVAR_ash_pGAS

	// GAS LUT
	//							//1030	//1027
	{0x002A, 0x2388},
	{0x0F12, 0x01C9},	//#TVAR_ash_pGAS[0]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[1]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[2]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[3]
	{0x0F12, 0x0105},	//#TVAR_ash_pGAS[4]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[5]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[6]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[7]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[8]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[9]
	{0x0F12, 0x0160},	//#TVAR_ash_pGAS[10]
	{0x0F12, 0x0189},	//#TVAR_ash_pGAS[11]
	{0x0F12, 0x01D0},	//#TVAR_ash_pGAS[12]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[13]
	{0x0F12, 0x016A},	//#TVAR_ash_pGAS[14]
	{0x0F12, 0x0126},	//#TVAR_ash_pGAS[15]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[16]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[17]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[18]
	{0x0F12, 0x009B},	//#TVAR_ash_pGAS[19]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[20]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[21]
	{0x0F12, 0x00EF},	//#TVAR_ash_pGAS[22]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[23]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[24]
	{0x0F12, 0x019C},	//#TVAR_ash_pGAS[25]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[26]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[27]
	{0x0F12, 0x00E2},	//#TVAR_ash_pGAS[28]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[29]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[30]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[31]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[32]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[33]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[34]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[35]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[36]
	{0x0F12, 0x0133},	//#TVAR_ash_pGAS[37]
	{0x0F12, 0x016E},	//#TVAR_ash_pGAS[38]
	{0x0F12, 0x0140},	//#TVAR_ash_pGAS[39]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[40]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[41]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[42]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[43]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[44]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[45]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[46]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[47]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[48]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[49]
	{0x0F12, 0x010D},	//#TVAR_ash_pGAS[50]
	{0x0F12, 0x0157},	//#TVAR_ash_pGAS[51]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[52]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[53]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[54]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[55]
	{0x0F12, 0x002F},	//#TVAR_ash_pGAS[56]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[57]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[58]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[59]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[60]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[61]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[62]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[63]
	{0x0F12, 0x0142},	//#TVAR_ash_pGAS[64]
	{0x0F12, 0x011A},	//#TVAR_ash_pGAS[65]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[66]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[67]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[68]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[69]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[70]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[71]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[72]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[73]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[74]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[75]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[76]
	{0x0F12, 0x013B},	//#TVAR_ash_pGAS[77]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[78]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[79]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[80]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[81]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[82]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[83]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[84]
	{0x0F12, 0x0014},	//#TVAR_ash_pGAS[85]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[86]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[87]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[88]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[89]
	{0x0F12, 0x013F},	//#TVAR_ash_pGAS[90]
	{0x0F12, 0x012C},	//#TVAR_ash_pGAS[91]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[92]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[93]
	{0x0F12, 0x0067},	//#TVAR_ash_pGAS[94]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[95]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[96]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[97]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[98]
	{0x0F12, 0x004B},	//#TVAR_ash_pGAS[99]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[100]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[101]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[102]
	{0x0F12, 0x0150},	//#TVAR_ash_pGAS[103]
	{0x0F12, 0x0149},	//#TVAR_ash_pGAS[104]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[105]
	{0x0F12, 0x00C5},	//#TVAR_ash_pGAS[106]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[107]
	{0x0F12, 0x0061},	//#TVAR_ash_pGAS[108]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[109]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[110]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[111]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[112]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[113]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[114]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[115]
	{0x0F12, 0x016D},	//#TVAR_ash_pGAS[116]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[117]
	{0x0F12, 0x013A},	//#TVAR_ash_pGAS[118]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[119]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[120]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[121]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[122]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[123]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[124]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[125]
	{0x0F12, 0x00D8},	//#TVAR_ash_pGAS[126]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[127]
	{0x0F12, 0x0154},	//#TVAR_ash_pGAS[128]
	{0x0F12, 0x0186},	//#TVAR_ash_pGAS[129]
	{0x0F12, 0x018F},	//#TVAR_ash_pGAS[130]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[131]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[132]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[133]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[134]
	{0x0F12, 0x00B4},	//#TVAR_ash_pGAS[135]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[136]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[137]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[138]
	{0x0F12, 0x0112},	//#TVAR_ash_pGAS[139]
	{0x0F12, 0x014F},	//#TVAR_ash_pGAS[140]
	{0x0F12, 0x0185},	//#TVAR_ash_pGAS[141]
	{0x0F12, 0x01A1},	//#TVAR_ash_pGAS[142]
	{0x0F12, 0x0168},	//#TVAR_ash_pGAS[143]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[144]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[145]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[146]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[147]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[148]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[149]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[150]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[151]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[152]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[153]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[154]
	{0x0F12, 0x0163},	//#TVAR_ash_pGAS[155]
	{0x0F12, 0x0131},	//#TVAR_ash_pGAS[156]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[157]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[158]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[159]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[160]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[161]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[162]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[163]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[164]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[165]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[166]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[167]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[168]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[169]
	{0x0F12, 0x00D2},	//#TVAR_ash_pGAS[170]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[171]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[172]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[173]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[174]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[175]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[176]
	{0x0F12, 0x0059},	//#TVAR_ash_pGAS[177]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[178]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[179]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[180]
	{0x0F12, 0x010C},	//#TVAR_ash_pGAS[181]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[182]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[183]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[184]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[185]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[186]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[187]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[188]
	{0x0F12, 0x0025},	//#TVAR_ash_pGAS[189]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[190]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[191]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[192]
	{0x0F12, 0x00C2},	//#TVAR_ash_pGAS[193]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[194]
	{0x0F12, 0x00DD},	//#TVAR_ash_pGAS[195]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[196]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[197]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[198]
	{0x0F12, 0x001E},	//#TVAR_ash_pGAS[199]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[200]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[201]
	{0x0F12, 0x0010},	//#TVAR_ash_pGAS[202]
	{0x0F12, 0x0026},	//#TVAR_ash_pGAS[203]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[204]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[205]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[206]
	{0x0F12, 0x00EC},	//#TVAR_ash_pGAS[207]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[208]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[209]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[210]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[211]
	{0x0F12, 0x0017},	//#TVAR_ash_pGAS[212]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[213]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[214]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[215]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[216]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[217]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[218]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[219]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[220]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[221]
	{0x0F12, 0x009A},	//#TVAR_ash_pGAS[222]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[223]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[224]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[225]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[226]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[227]
	{0x0F12, 0x000F},	//#TVAR_ash_pGAS[228]
	{0x0F12, 0x0027},	//#TVAR_ash_pGAS[229]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[230]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[231]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[232]
	{0x0F12, 0x00F1},	//#TVAR_ash_pGAS[233]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[234]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[235]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[236]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[237]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[238]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[239]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[240]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[241]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[242]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[243]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[244]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[245]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[246]
	{0x0F12, 0x00FA},	//#TVAR_ash_pGAS[247]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[248]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[249]
	{0x0F12, 0x0064},	//#TVAR_ash_pGAS[250]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[251]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[252]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[253]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[254]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[255]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[256]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[257]
	{0x0F12, 0x00DF},	//#TVAR_ash_pGAS[258]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[259]
	{0x0F12, 0x0110},	//#TVAR_ash_pGAS[260]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[261]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[262]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[263]
	{0x0F12, 0x006E},	//#TVAR_ash_pGAS[264]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[265]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[266]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[267]
	{0x0F12, 0x0083},	//#TVAR_ash_pGAS[268]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[269]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[270]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[271]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[272]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[273]
	{0x0F12, 0x0104},	//#TVAR_ash_pGAS[274]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[275]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[276]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[277]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[278]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[279]
	{0x0F12, 0x0098},	//#TVAR_ash_pGAS[280]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[281]
	{0x0F12, 0x00D1},	//#TVAR_ash_pGAS[282]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[283]
	{0x0F12, 0x0127},	//#TVAR_ash_pGAS[284]
	{0x0F12, 0x0146},	//#TVAR_ash_pGAS[285]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[286]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[287]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[288]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[289]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[290]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[291]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[292]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[293]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[294]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[295]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[296]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[297]
	{0x0F12, 0x0180},	//#TVAR_ash_pGAS[298]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[299]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[300]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[301]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[302]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[303]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[304]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[305]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[306]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[307]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[308]
	{0x0F12, 0x00EE},	//#TVAR_ash_pGAS[309]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[310]
	{0x0F12, 0x014C},	//#TVAR_ash_pGAS[311]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[312]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[313]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[314]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[315]
	{0x0F12, 0x0057},	//#TVAR_ash_pGAS[316]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[317]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[318]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[319]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[320]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[321]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[322]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[323]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[324]
	{0x0F12, 0x00F6},	//#TVAR_ash_pGAS[325]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[326]
	{0x0F12, 0x0081},	//#TVAR_ash_pGAS[327]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[328]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[329]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[330]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[331]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[332]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[333]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[334]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[335]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[336]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[337]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[338]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[339]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[340]
	{0x0F12, 0x0041},	//#TVAR_ash_pGAS[341]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[342]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[343]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[344]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[345]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[346]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[347]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[348]
	{0x0F12, 0x00BA},	//#TVAR_ash_pGAS[349]
	{0x0F12, 0x00F7},	//#TVAR_ash_pGAS[350]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[351]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[352]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[353]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[354]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[355]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[356]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[357]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[358]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[359]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[360]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[361]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[362]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[363]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[364]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[365]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[366]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[367]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[368]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[369]
	{0x0F12, 0x0002},	//#TVAR_ash_pGAS[370]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[371]
	{0x0F12, 0x0023},	//#TVAR_ash_pGAS[372]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[373]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[374]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[375]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[376]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[377]
	{0x0F12, 0x00B0},	//#TVAR_ash_pGAS[378]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[379]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[380]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[381]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[382]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[383]
	{0x0F12, 0x001C},	//#TVAR_ash_pGAS[384]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[385]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[386]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[387]
	{0x0F12, 0x00B7},	//#TVAR_ash_pGAS[388]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[389]
	{0x0F12, 0x0106},	//#TVAR_ash_pGAS[390]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[391]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[392]
	{0x0F12, 0x006C},	//#TVAR_ash_pGAS[393]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[394]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[395]
	{0x0F12, 0x0033},	//#TVAR_ash_pGAS[396]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[397]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[398]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[399]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[400]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[401]
	{0x0F12, 0x0101},	//#TVAR_ash_pGAS[402]
	{0x0F12, 0x0120},	//#TVAR_ash_pGAS[403]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[404]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[405]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[406]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[407]
	{0x0F12, 0x0062},	//#TVAR_ash_pGAS[408]
	{0x0F12, 0x005C},	//#TVAR_ash_pGAS[409]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[410]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[411]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[412]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[413]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[414]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[415]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[416]
	{0x0F12, 0x0115},	//#TVAR_ash_pGAS[417]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[418]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[419]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[420]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[421]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[422]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[423]
	{0x0F12, 0x00A4},	//#TVAR_ash_pGAS[424]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[425]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[426]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[427]
	{0x0F12, 0x0130},	//#TVAR_ash_pGAS[428]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[429]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[430]
	{0x0F12, 0x00C6},	//#TVAR_ash_pGAS[431]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[432]
	{0x0F12, 0x0090},	//#TVAR_ash_pGAS[433]
	{0x0F12, 0x0085},	//#TVAR_ash_pGAS[434]
	{0x0F12, 0x0082},	//#TVAR_ash_pGAS[435]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[436]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[437]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[438]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[439]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[440]
	{0x0F12, 0x0135},	//#TVAR_ash_pGAS[441]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[442]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[443]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[444]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[445]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[446]
	{0x0F12, 0x005D},	//#TVAR_ash_pGAS[447]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[448]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[449]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[450]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[451]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[452]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[453]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[454]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[455]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[456]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[457]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[458]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[459]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[460]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[461]
	{0x0F12, 0x003D},	//#TVAR_ash_pGAS[462]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[463]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[464]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[465]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[466]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[467]
	{0x0F12, 0x00B5},	//#TVAR_ash_pGAS[468]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[469]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[470]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[471]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[472]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[473]
	{0x0F12, 0x001A},	//#TVAR_ash_pGAS[474]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[475]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[476]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[477]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[478]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[479]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[480]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[481]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[482]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[483]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[484]
	{0x0F12, 0x0018},	//#TVAR_ash_pGAS[485]
	{0x0F12, 0x000A},	//#TVAR_ash_pGAS[486]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[487]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[488]
	{0x0F12, 0x001F},	//#TVAR_ash_pGAS[489]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[490]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[491]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[492]
	{0x0F12, 0x00B6},	//#TVAR_ash_pGAS[493]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[494]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[495]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[496]
	{0x0F12, 0x0028},	//#TVAR_ash_pGAS[497]
	{0x0F12, 0x0012},	//#TVAR_ash_pGAS[498]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[499]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[500]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[501]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[502]
	{0x0F12, 0x0030},	//#TVAR_ash_pGAS[503]
	{0x0F12, 0x0050},	//#TVAR_ash_pGAS[504]
	{0x0F12, 0x007B},	//#TVAR_ash_pGAS[505]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[506]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[507]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[508]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[509]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[510]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[511]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[512]
	{0x0F12, 0x0001},	//#TVAR_ash_pGAS[513]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[514]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[515]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[516]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[517]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[518]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[519]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[520]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[521]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[522]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[523]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[524]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[525]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[526]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[527]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[528]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[529]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[530]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[531]
	{0x0F12, 0x00AE},	//#TVAR_ash_pGAS[532]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[533]
	{0x0F12, 0x0097},	//#TVAR_ash_pGAS[534]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[535]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[536]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[537]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[538]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[539]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[540]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[541]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[542]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[543]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[544]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[545]
	{0x0F12, 0x00D3},	//#TVAR_ash_pGAS[546]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[547]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[548]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[549]
	{0x0F12, 0x0058},	//#TVAR_ash_pGAS[550]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[551]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[552]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[553]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[554]
	{0x0F12, 0x006D},	//#TVAR_ash_pGAS[555]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[556]
	{0x0F12, 0x00AC},	//#TVAR_ash_pGAS[557]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[558]
	{0x0F12, 0x00F5},	//#TVAR_ash_pGAS[559]
	{0x0F12, 0x00D4},	//#TVAR_ash_pGAS[560]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[561]
	{0x0F12, 0x008E},	//#TVAR_ash_pGAS[562]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[563]
	{0x0F12, 0x0070},	//#TVAR_ash_pGAS[564]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[565]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[566]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[567]
	{0x0F12, 0x0093},	//#TVAR_ash_pGAS[568]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[569]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[570]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[571]



	//=================================================================================================
	//	Set Gamma LUT
	//=================================================================================================
	//							//New	//Hong	//ORG
	{0x002A, 0x04CC},
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[0][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[0][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[0][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[0][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[0][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[0][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[0][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[0][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[0][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[0][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[0][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[0][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[0][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[0][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[0][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[0][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[1][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[1][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[1][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[1][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[1][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[1][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[1][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[1][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[1][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[1][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[1][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[1][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[1][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[1][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[1][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[1][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[2][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[2][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[2][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[2][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[2][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[2][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[2][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[2][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[2][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[2][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[2][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[2][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[2][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[2][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[2][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[2][15]



	//=================================================================================================
	//	Set AWB
	//=================================================================================================
	{0x002A, 0x0DA8},
	{0x0F12, 0x0020},	// #awbb_LowBr0_NBzone	// 2008.04.24. eliminate to the purple color at low light.
	{0x002A, 0x0E8C},
	{0x0F12, 0x0000},	// #awbb_LowBr0_PatchNumZone
	{0x002A, 0x0D6C},
	{0x0F12, 0x0040},	// #awbb_YMedMoveToYAv

	// Indoor Gray Zone
	{0x002A, 0x0B9C},
	{0x0F12, 0x0339},	//0392	// #awbb_IndoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x0393},	//03A4	// #awbb_IndoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x031B},	//036F	// #awbb_IndoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x039C},	//03AE	// #awbb_IndoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x030A},	//034E	// #awbb_IndoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0398},	//03A2	// #awbb_IndoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x02F3},	//0331	// #awbb_IndoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0392},	//0392	// #awbb_IndoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x02E0},	//0315	// #awbb_IndoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0388},	//0381	// #awbb_IndoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x02CB},	//02F8	// #awbb_IndoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0374},	//036C	// #awbb_IndoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x02BB},	//02E1	// #awbb_IndoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x034B},	//0358	// #awbb_IndoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x02AA},	//02C5	// #awbb_IndoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x0336},	//033B	// #awbb_IndoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x029C},	//02B1	// #awbb_IndoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0321},	//0323	// #awbb_IndoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x028B},	//029C	// #awbb_IndoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x030B},	//030F	// #awbb_IndoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x027B},	//028B	// #awbb_IndoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02FD},	//02FA	// #awbb_IndoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x0268},	//027C	// #awbb_IndoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x02EA},	//02E2	// #awbb_IndoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x0259},	//0268	// #awbb_IndoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x02D8},	//02D1	// #awbb_IndoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x024C},	//0259	// #awbb_IndoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x02CB},	//02BF	// #awbb_IndoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x023D},	//024C	// #awbb_IndoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x02BD},	//02B1	// #awbb_IndoorGrZones_m_BGrid_14__m_right
	{0x0F12, 0x0231},	//023D	// #awbb_IndoorGrZones_m_BGrid_15__m_left
	{0x0F12, 0x02B1},	//02A2	// #awbb_IndoorGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0222},	//0231	// #awbb_IndoorGrZones_m_BGrid_16__m_left
	{0x0F12, 0x02A8},	//0299	// #awbb_IndoorGrZones_m_BGrid_16__m_right
	{0x0F12, 0x0215},	//0222	// #awbb_IndoorGrZones_m_BGrid_17__m_left
	{0x0F12, 0x029B},	//0290	// #awbb_IndoorGrZones_m_BGrid_17__m_right
	{0x0F12, 0x0208},	//0215	// #awbb_IndoorGrZones_m_BGrid_18__m_left
	{0x0F12, 0x0290},	//0288	// #awbb_IndoorGrZones_m_BGrid_18__m_right
	{0x0F12, 0x01F7},	//0208	// #awbb_IndoorGrZones_m_BGrid_19__m_left
	{0x0F12, 0x0287},	//0281	// #awbb_IndoorGrZones_m_BGrid_19__m_right
	{0x0F12, 0x01E6},	//01F7	// #awbb_IndoorGrZones_m_BGrid_20__m_left
	{0x0F12, 0x027F},	//0279	// #awbb_IndoorGrZones_m_BGrid_20__m_right
	{0x0F12, 0x01D9},	//01E6	// #awbb_IndoorGrZones_m_BGrid_21__m_left
	{0x0F12, 0x0279},	//026E	// #awbb_IndoorGrZones_m_BGrid_21__m_right
	{0x0F12, 0x01CC},	//01D9	// #awbb_IndoorGrZones_m_BGrid_22__m_left
	{0x0F12, 0x0272},	//0268	// #awbb_IndoorGrZones_m_BGrid_22__m_right
	{0x0F12, 0x01C1},	//01CC	// #awbb_IndoorGrZones_m_BGrid_23__m_left
	{0x0F12, 0x026C},	//0263	// #awbb_IndoorGrZones_m_BGrid_23__m_right
	{0x0F12, 0x01B3},	//01C1	// #awbb_IndoorGrZones_m_BGrid_24__m_left
	{0x0F12, 0x0265},	//0260	// #awbb_IndoorGrZones_m_BGrid_24__m_right
	{0x0F12, 0x01AA},	//01B3	// #awbb_IndoorGrZones_m_BGrid_25__m_left
	{0x0F12, 0x025C},	//0259	// #awbb_IndoorGrZones_m_BGrid_25__m_right
	{0x0F12, 0x019F},	//01AA	// #awbb_IndoorGrZones_m_BGrid_26__m_left
	{0x0F12, 0x0250},	//0253	// #awbb_IndoorGrZones_m_BGrid_26__m_right
	{0x0F12, 0x019B},	//019F	// #awbb_IndoorGrZones_m_BGrid_27__m_left
	{0x0F12, 0x0247},	//0248	// #awbb_IndoorGrZones_m_BGrid_27__m_right
	{0x0F12, 0x0197},	//019B	// #awbb_IndoorGrZones_m_BGrid_28__m_left
	{0x0F12, 0x0239},	//023F	// #awbb_IndoorGrZones_m_BGrid_28__m_right
	{0x0F12, 0x019B},	//0197	// #awbb_IndoorGrZones_m_BGrid_29__m_left
	{0x0F12, 0x021D},	//0239	// #awbb_IndoorGrZones_m_BGrid_29__m_right
	{0x0F12, 0x019D},	//019B	// #awbb_IndoorGrZones_m_BGrid_30__m_left
	{0x0F12, 0x0202},	//021D	// #awbb_IndoorGrZones_m_BGrid_30__m_right
	{0x0F12, 0x01AC},	//019D	// #awbb_IndoorGrZones_m_BGrid_31__m_left
	{0x0F12, 0x01D3},	//020A	// #awbb_IndoorGrZones_m_BGrid_31__m_right
	{0x0F12, 0x0000},	//01AC	// #awbb_IndoorGrZones_m_BGrid_32__m_left
	{0x0F12, 0x0000},	//01D3	// #awbb_IndoorGrZones_m_BGrid_32__m_right
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_left
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_right

	{0x002A, 0x0C3C},
	{0x0F12, 0x0004},	// #awbb_IndoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0138},	// #awbb_IndoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x0F12, 0x0000},

	// Outdoor Gray Zone
	{0x0F12, 0x0255},	//#awbb_OutdoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x0236},	//#awbb_OutdoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x0290},	//#awbb_OutdoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x0227},	//#awbb_OutdoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0297},	//#awbb_OutdoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x0210},	//#awbb_OutdoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0293},	//#awbb_OutdoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0200},	//#awbb_OutdoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x028B},	//#awbb_OutdoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x01F1},	//#awbb_OutdoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0286},	//#awbb_OutdoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x0282},	//#awbb_OutdoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x01E0},	//#awbb_OutdoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x027B},	//#awbb_OutdoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x01DE},	//#awbb_OutdoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0274},	//#awbb_OutdoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01DF},	//#awbb_OutdoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x0263},	//#awbb_OutdoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01F6},	//#awbb_OutdoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x0253},	//#awbb_OutdoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x020B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x023B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_right

	{0x002A, 0x0CB8},
	{0x0F12, 0x0004},	// #awbb_OutdoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x000C},	//#awbb_OutdoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0217},	//#awbb_OutdoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high
	{0x0F12, 0x0000},

	// Low Brightness Gray Zone
	{0x0F12, 0x0324},	//031E  // #awbb_LowBrGrZones_m_BGrid_0__m_left
	{0x0F12, 0x03A1},	//037E  // #awbb_LowBrGrZones_m_BGrid_0__m_right
	{0x0F12, 0x02F2},	//02CF  // #awbb_LowBrGrZones_m_BGrid_1__m_left
	{0x0F12, 0x03BA},	//0389  // #awbb_LowBrGrZones_m_BGrid_1__m_right
	{0x0F12, 0x02C4},	//02A6  // #awbb_LowBrGrZones_m_BGrid_2__m_left
	{0x0F12, 0x03B6},	//0386  // #awbb_LowBrGrZones_m_BGrid_2__m_right
	{0x0F12, 0x029E},	//0282  // #awbb_LowBrGrZones_m_BGrid_3__m_left
	{0x0F12, 0x03A6},	//0374  // #awbb_LowBrGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0283},	//0259  // #awbb_LowBrGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0380},	//0352  // #awbb_LowBrGrZones_m_BGrid_4__m_right
	{0x0F12, 0x025F},	//0239  // #awbb_LowBrGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0344},	//0330  // #awbb_LowBrGrZones_m_BGrid_5__m_right
	{0x0F12, 0x0241},	//021E  // #awbb_LowBrGrZones_m_BGrid_6__m_left
	{0x0F12, 0x031F},	//031B  // #awbb_LowBrGrZones_m_BGrid_6__m_right
	{0x0F12, 0x0227},	//020B  // #awbb_LowBrGrZones_m_BGrid_7__m_left
	{0x0F12, 0x02FC},	//0300  // #awbb_LowBrGrZones_m_BGrid_7__m_right
	{0x0F12, 0x020A},	//01F1  // #awbb_LowBrGrZones_m_BGrid_8__m_left
	{0x0F12, 0x02D9},	//02DD  // #awbb_LowBrGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01F1},	//01E0  // #awbb_LowBrGrZones_m_BGrid_9__m_left
	{0x0F12, 0x02C4},	//02C4  // #awbb_LowBrGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01D0},	//01CD  // #awbb_LowBrGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02B2},	//02AF  // #awbb_LowBrGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01B8},	//019E  // #awbb_LowBrGrZones_m_BGrid_11__m_left
	{0x0F12, 0x029E},	//02A0  // #awbb_LowBrGrZones_m_BGrid_11__m_right
	{0x0F12, 0x019F},	//0187  // #awbb_LowBrGrZones_m_BGrid_12__m_left
	{0x0F12, 0x0290},	//028C  // #awbb_LowBrGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0189},	//017D  // #awbb_LowBrGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0282},	//027E  // #awbb_LowBrGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0182},	//0179  // #awbb_LowBrGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0271},	//0258  // #awbb_LowBrGrZones_m_BGrid_14__m_right
	{0x0F12, 0x017F},	//0186  // #awbb_LowBrGrZones_m_BGrid_15__m_left
	{0x0F12, 0x0254},	//0232  // #awbb_LowBrGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0183},	//01BD  // #awbb_LowBrGrZones_m_BGrid_16__m_left
	{0x0F12, 0x022D},	//01FB  // #awbb_LowBrGrZones_m_BGrid_16__m_right
	{0x0F12, 0x019C},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_left
	{0x0F12, 0x01D3},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_right

	{0x002A, 0x0D34},
	{0x0F12, 0x0005},	// #awbb_LowBrGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0012},	// #awbb_LowBrGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0120},	// #awbb_LowBrGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0002},	// #awbb_LowBrGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D48},
	{0x0F12, 0x00E0},	// #awbb_LowBrGrZones_y_high
	{0x0F12, 0x0000},

	// Lowtemp Circle
	{0x0F12, 0x032F},	// #awbb_CrclLowT_R_c
	{0x0F12, 0x0000},
	{0x0F12, 0x017A},	// #awbb_CrclLowT_B_c
	{0x0F12, 0x0000},
	{0x0F12, 0x7300},	// #awbb_CrclLowT_Rad_c
	{0x0F12, 0x0000},
	{0x0F12, 0x000A},	// #awbb_CrclLowT_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D60},
	{0x0F12, 0x00E0},	// #awbb_CrclLowT_y_high
	{0x0F12, 0x0000},
	{0x002A, 0x0D82},
	{0x0F12, 0x0001},	// #awbb_ByPass_LowTempMode

	// White Locus
	{0x002A, 0x0D66},
	{0x0F12, 0x011F},	// #awbb_IntcR
	{0x0F12, 0x0132},	// #awbb_IntcB
	{0x002A, 0x0D74},
	{0x0F12, 0x052A},	// #awbb_MvEq_RBthresh

	// Gamut Thresholds
	{0x002A, 0x0DAE},
	{0x0F12, 0x0036},	// #awbb_GamutWidthThr2
	{0x0F12, 0x001C},	// #awbb_GamutHeightThr2
	{0x002A, 0x0DAA},
	{0x0F12, 0x071A},	// #awbb_GamutWidthThr1
	{0x0F12, 0x03A4},	// #awbb_GamutHeightThr1

	// SceneDetection Thresholds
	{0x002A, 0x0D92},
	{0x0F12, 0x0CE4},	//0A8C	// #awbb_SunnyBr
	{0x0F12, 0x0096},	// #awbb_Sunny_NBzone
	{0x002A, 0x0E86},
	{0x0F12, 0x0216},	// #awbb_OutdoorWP_r
	{0x0F12, 0x029F},	// #awbb_OutdoorWP_b
	{0x002A, 0x0D96},
	{0x0F12, 0x0580},	// #awbb_CloudyBr
	{0x0F12, 0x0096},	// #awbb_Cloudy_NBzone
	{0x002A, 0x0DB2},
	{0x0F12, 0x00DA},	// #awbb_CloudyRB
	{0x002A, 0x0D9A},
	{0x0F12, 0x000A},	// #awbb_Cloudy_BdivRzone
	{0x002A, 0x0DB4},
	{0x0F12, 0x0459},	// #awbb_LowTempRB
	{0x002A, 0x0DA4},
	{0x0F12, 0x000E},	// #awbb_LowTemp_RBzone
	{0x002A, 0x0D64},
	{0x0F12, 0x0040},	// #awbb_DarkBr
	{0x002A, 0x0DA6},
	{0x0F12, 0x001E},	// #awbb_LowBr_NBzone
	{0x002A, 0x0D9C},
	{0x0F12, 0x001B},	// #awbb_MacbethGamut_WidthZone
	{0x0F12, 0x000E},	// #awbb_MacbethGamut_HeightZone
	{0x0F12, 0x0008},	// #awbb_MacbethGamut_WidthZone2
	{0x0F12, 0x0004},	// #awbb_MacbethGamut_HeightZone2

	// AWB Debug.(Outdoor Pink)
	{0x002A, 0x0E30},
	{0x0F12, 0x0000},	//	#awbb_OutdoorFltrSz (outdoor WB를 moving average filtering)

	// UseInvalidOutdoor option
	{0x002A, 0x0D88},
	{0x0F12, 0x0001},	// #awbb_Use_InvalidOutDoor

	// AWB input Y-Filter setting
	{0x002A, 0x0C48},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x002A, 0x0CC4},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high

	{0x002A, 0x0DC2},
	{0x0F12, 0x0030},	// #awbb_GnCurPntImmunity
	{0x0F12, 0x00C8},	// #awbb_GnFarPntImmunity
	{0x0F12, 0x012C},	// #awbb_GnCurPntLongJump
	{0x0F12, 0x0258},	// #awbb_GainsMaxMove
	{0x0F12, 0x0003},	// #awbb_GnMinMatchToJump

	// RB Gain Offset
	//WRITE 70000E32	FFE0	//#awbb_RGainOff
	//WRITE 70000E34	FFe0	//#awbb_BGainOff


	//=================================================================================================
	//	Set CCM
	//=================================================================================================
	// CCM Start Address
	{0x002A, 0x06D0},
	{0x0F12, 0x2800},   //#TVAR_wbt_pBaseCcmsAddr[0]
	{0x0F12, 0x7000},
	{0x0F12, 0x2824},   //#TVAR_wbt_pBaseCcmsAddr[1]
	{0x0F12, 0x7000},
	{0x0F12, 0x2848},   //#TVAR_wbt_pBaseCcmsAddr[2]
	{0x0F12, 0x7000},
	{0x0F12, 0x286C},   //#TVAR_wbt_pBaseCcmsAddr[3]
	{0x0F12, 0x7000},
	{0x0F12, 0x2890},   //#TVAR_wbt_pBaseCcmsAddr[4]
	{0x0F12, 0x7000},
	{0x0F12, 0x28B4},   //#TVAR_wbt_pBaseCcmsAddr[5]
	{0x0F12, 0x7000},
	{0x002A, 0x06EC},
	{0x0F12, 0x28D8},   //#TVAR_wbt_pOutdoorCcm
	{0x0F12, 0x7000},

	{0x002A, 0x2800},
	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	// Outdoor CCM
	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEA4},
	{0x0F12, 0x020F},
	{0x0F12, 0xFF8F},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},



	//=================================================================================================
	//	Set NB
	//=================================================================================================
	{0x002A, 0x07EA},
	{0x0F12, 0x0000},	//afit_bUseNoiseInd 0 : NB 1: Noise Index

	// param_start	SARR_uNormBrInDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrInDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrInDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrInDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrInDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrInDoor[4]

	// param_start	SARR_uNormBrOutDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrOutDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrOutDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrOutDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrOutDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrOutDoor[4]



	//=================================================================================================
	//	Set AFIT
	//=================================================================================================
	// AFIT Start Address
	{0x002A, 0x0814},
	{0x0F12, 0x082C},	//#TVAR_afit_pBaseVals
	{0x0F12, 0x7000},	//#TVAR_afit_pBaseVals

	// AFIT Table	//0223
	{0x002A, 0x082C},
	{0x0F12, 0x0010},	//BRIGHTNESS
	{0x0F12, 0xFFF0},	//CONTRAST
	{0x0F12, 0xFFD0},	//SATURATION
	{0x0F12, 0x0000},	//SHARP_BLUR
	{0x0F12, 0x0000},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x003B},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                   bin: desparity low
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"                bin: desparity high
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x06FF},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x030A},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"               ]negati fine
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"                  ]low fine
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"                ]high fine
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"                   ]high low thres
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0005},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01AA},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0006},	//BRIGHTNESS
	{0x0F12, 0x0000},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0000},	//SHARP_BLUR
	{0x0F12, 0x0000},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0021},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                  bin: desparity low
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x08B7},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0308},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01A0},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0021},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0089},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0202},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0202},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0206},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x803C},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0019},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0205},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0000},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8050},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0019},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0000},	//UVDenoise_iYLowThresh
	{0x0F12, 0x0000},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0105},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0200},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x805A},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[0]
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[1]
	{0x0F12, 0x0800},	// # afit_pConstBaseVals[2]
	{0x0F12, 0x0600},	// # afit_pConstBaseVals[3]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[4]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[5]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[6]
	{0x0F12, 0x0300},	// # afit_pConstBaseVals[7]
	{0x0F12, 0x0002},	// # afit_pConstBaseVals[8]
	{0x0F12, 0x0400},	// # afit_pConstBaseVals[9]
	{0x0F12, 0x0106},	// # afit_pConstBaseVals[10]
	{0x0F12, 0x0005},	// # afit_pConstBaseVals[11]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[12]
	{0x0F12, 0x0003},	// # afit_pConstBaseVals[13]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[14]
	{0x0F12, 0xFFD6},	// # afit_pConstBaseVals[15]
	{0x0F12, 0x53C1},	// # afit_pConstBaseVals[16]
	{0x0F12, 0xE1FE},	// # afit_pConstBaseVals[17]
	{0x0F12, 0x0001},	// # afit_pConstBaseVals[18]

	{0x002A, 0x01EA},
	{0x0F12, 0x0000},	//#REG_TC_UserSharpBlur
	{0x002A, 0x0E32},
	{0x0F12, 0xFFF0},	//#awbb_RGainOff
	{0x0F12, 0x0000},	//#awbb_BGainOff

	// For fast AE
	{0x002A, 0x0FAC},
	{0x0F12, 0x0001},	// AE Gain output [0]    default : 0x10
	{0x0F12, 0x0008},	// AE Gain output [1]    default : 0x20
	{0x0F12, 0x0015},	// AE Gain output [2]    default : 0x40

	// For fast AWB
	{0x002A, 0x0DCA},
	{0x0F12, 0x0001},	// awbb_GnMinMatchToJump = 0x02 (default:0x03)
	{0x0028, 0x7000},
	{0x002A, 0x0E98},
	{0x0F12, 0x0300},	//bp_uMaxBrightnessFactor 0180 -> 0300
	{0x002A, 0x0E9E},
	{0x0F12, 0x0300},	//bp_uMinBrightnessFactor 0180 -> 0300

	// Update Changed Registers
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},

	{0x0028, 0xD000},
	{0x002A, 0x1088},
	{0x0F12, 0x05D5},

	{0x0028, 0xD000},
	{0x002A, 0x1094},
	{0x0F12, 0x0010},

	{0x0028, 0xD000},
	{0x002A, 0x1082},
	{0x0F12, 0x03FF},

	{0x0028, 0xD000},
	{0x002A, 0x1084},
	{0x0F12, 0x03FF},

    {SEQUENCE_WAIT_MS, 0x0064},
    {SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_init_800x600[] =
{
	{0x0028, 0xD000},
	{0x002A, 0x1030},
	{0x0F12, 0x0000},
	{0x002A, 0x0014},
	{0x0F12, 0x0001},

	{SEQUENCE_WAIT_MS , 0x0014},// Delay 100 -> 20

	{0x0028, 0x7000},
	{0x002A, 0x2080},
	{0x0F12, 0xB510},	// 700020800
	{0x0F12, 0xF000},	// 700020822
	{0x0F12, 0xF8FC},	// 700020844
	{0x0F12, 0xBC10},	// 700020866
	{0x0F12, 0xBC08},	// 700020888
	{0x0F12, 0x4718},	// 7000208AA
	{0x0F12, 0xB5FE},	// 7000208CC
	{0x0F12, 0x0007},	// 7000208EE
	{0x0F12, 0x683C},	// 700020900
	{0x0F12, 0x687E},	// 700020922
	{0x0F12, 0x1DA5},	// 700020944
	{0x0F12, 0x88A0},	// 700020966
	{0x0F12, 0x2800},	// 700020988
	{0x0F12, 0xD00B},	// 7000209AA
	{0x0F12, 0x88A8},	// 7000209CC
	{0x0F12, 0x2800},	// 7000209EE
	{0x0F12, 0xD008},	// 700020A00
	{0x0F12, 0x8820},	// 700020A22
	{0x0F12, 0x8829},	// 700020A44
	{0x0F12, 0x4288},	// 700020A66
	{0x0F12, 0xD301},	// 700020A88
	{0x0F12, 0x1A40},	// 700020AAA
	{0x0F12, 0xE000},	// 700020ACC
	{0x0F12, 0x1A08},	// 700020AEE
	{0x0F12, 0x9001},	// 700020B00
	{0x0F12, 0xE001},	// 700020B22
	{0x0F12, 0x2019},	// 700020B44
	{0x0F12, 0x9001},	// 700020B66
	{0x0F12, 0x497B},	// 700020B88
	{0x0F12, 0x466B},	// 700020BAA
	{0x0F12, 0x8A48},	// 700020BCC
	{0x0F12, 0x8118},	// 700020BEE
	{0x0F12, 0x8A88},	// 700020C00
	{0x0F12, 0x8158},	// 700020C22
	{0x0F12, 0x4879},	// 700020C44
	{0x0F12, 0x8940},	// 700020C66
	{0x0F12, 0x0040},	// 700020C88
	{0x0F12, 0x2103},	// 700020CAA
	{0x0F12, 0xF000},	// 700020CCC
	{0x0F12, 0xF902},	// 700020CEE
	{0x0F12, 0x88A1},	// 700020D00
	{0x0F12, 0x4288},	// 700020D22
	{0x0F12, 0xD908},	// 700020D44
	{0x0F12, 0x8828},	// 700020D66
	{0x0F12, 0x8030},	// 700020D88
	{0x0F12, 0x8868},	// 700020DAA
	{0x0F12, 0x8070},	// 700020DCC
	{0x0F12, 0x88A8},	// 700020DEE
	{0x0F12, 0x6038},	// 700020E00
	{0x0F12, 0xBCFE},	// 700020E22
	{0x0F12, 0xBC08},	// 700020E44
	{0x0F12, 0x4718},	// 700020E66
	{0x0F12, 0x88A9},	// 700020E88
	{0x0F12, 0x4288},	// 700020EAA
	{0x0F12, 0xD906},	// 700020ECC
	{0x0F12, 0x8820},	// 700020EEE
	{0x0F12, 0x8030},	// 700020F00
	{0x0F12, 0x8860},	// 700020F22
	{0x0F12, 0x8070},	// 700020F44
	{0x0F12, 0x88A0},	// 700020F66
	{0x0F12, 0x6038},	// 700020F88
	{0x0F12, 0xE7F2},	// 700020FAA
	{0x0F12, 0x9801},	// 700020FCC
	{0x0F12, 0xA902},	// 700020FEE
	{0x0F12, 0xF000},	// 700021000
	{0x0F12, 0xF8EE},	// 700021022
	{0x0F12, 0x0033},	// 700021044
	{0x0F12, 0x0029},	// 700021066
	{0x0F12, 0x9A02},	// 700021088
	{0x0F12, 0x0020},	// 7000210AA
	{0x0F12, 0xF000},	// 7000210CC
	{0x0F12, 0xF8F0},	// 7000210EE
	{0x0F12, 0x6038},	// 700021100
	{0x0F12, 0xE7E6},	// 700021122
	{0x0F12, 0xB5F0},	// 700021144
	{0x0F12, 0xB08B},	// 700021166
	{0x0F12, 0x0006},	// 700021188
	{0x0F12, 0x2000},	// 7000211AA
	{0x0F12, 0x9004},	// 7000211CC
	{0x0F12, 0x6835},	// 7000211EE
	{0x0F12, 0x6874},	// 700021200
	{0x0F12, 0x68B0},	// 700021222
	{0x0F12, 0x900A},	// 700021244
	{0x0F12, 0x68F0},	// 700021266
	{0x0F12, 0x9009},	// 700021288
	{0x0F12, 0x4F60},	// 7000212AA
	{0x0F12, 0x8979},	// 7000212CC
	{0x0F12, 0x084A},	// 7000212EE
	{0x0F12, 0x88A8},	// 700021300
	{0x0F12, 0x88A3},	// 700021322
	{0x0F12, 0x4298},	// 700021344
	{0x0F12, 0xD300},	// 700021366
	{0x0F12, 0x0018},	// 700021388
	{0x0F12, 0xF000},	// 7000213AA
	{0x0F12, 0xF8E1},	// 7000213CC
	{0x0F12, 0x9007},	// 7000213EE
	{0x0F12, 0x0021},	// 700021400
	{0x0F12, 0x0028},	// 700021422
	{0x0F12, 0xAA04},	// 700021444
	{0x0F12, 0xF000},	// 700021466
	{0x0F12, 0xF8E3},	// 700021488
	{0x0F12, 0x9006},	// 7000214AA
	{0x0F12, 0x88A8},	// 7000214CC
	{0x0F12, 0x2800},	// 7000214EE
	{0x0F12, 0xD102},	// 700021500
	{0x0F12, 0x27FF},	// 700021522
	{0x0F12, 0x1C7F},	// 700021544
	{0x0F12, 0xE047},	// 700021566
	{0x0F12, 0x88A0},	// 700021588
	{0x0F12, 0x2800},	// 7000215AA
	{0x0F12, 0xD101},	// 7000215CC
	{0x0F12, 0x2700},	// 7000215EE
	{0x0F12, 0xE042},	// 700021600
	{0x0F12, 0x8820},	// 700021622
	{0x0F12, 0x466B},	// 700021644
	{0x0F12, 0x8198},	// 700021666
	{0x0F12, 0x8860},	// 700021688
	{0x0F12, 0x81D8},	// 7000216AA
	{0x0F12, 0x8828},	// 7000216CC
	{0x0F12, 0x8118},	// 7000216EE
	{0x0F12, 0x8868},	// 700021700
	{0x0F12, 0x8158},	// 700021722
	{0x0F12, 0xA802},	// 700021744
	{0x0F12, 0xC803},	// 700021766
	{0x0F12, 0xF000},	// 700021788
	{0x0F12, 0xF8D2},	// 7000217AA
	{0x0F12, 0x9008},	// 7000217CC
	{0x0F12, 0x8ABA},	// 7000217EE
	{0x0F12, 0x9808},	// 700021800
	{0x0F12, 0x466B},	// 700021822
	{0x0F12, 0x4342},	// 700021844
	{0x0F12, 0x9202},	// 700021866
	{0x0F12, 0x8820},	// 700021888
	{0x0F12, 0x8198},	// 7000218AA
	{0x0F12, 0x8860},	// 7000218CC
	{0x0F12, 0x81D8},	// 7000218EE
	{0x0F12, 0x980A},	// 700021900
	{0x0F12, 0x9903},	// 700021922
	{0x0F12, 0xF000},	// 700021944
	{0x0F12, 0xF8C4},	// 700021966
	{0x0F12, 0x9A02},	// 700021988
	{0x0F12, 0x17D1},	// 7000219AA
	{0x0F12, 0x0E09},	// 7000219CC
	{0x0F12, 0x1889},	// 7000219EE
	{0x0F12, 0x1209},	// 700021A00
	{0x0F12, 0x4288},	// 700021A22
	{0x0F12, 0xDD1F},	// 700021A44
	{0x0F12, 0x8820},	// 700021A66
	{0x0F12, 0x466B},	// 700021A88
	{0x0F12, 0x8198},	// 700021AAA
	{0x0F12, 0x8860},	// 700021ACC
	{0x0F12, 0x81D8},	// 700021AEE
	{0x0F12, 0x980A},	// 700021B00
	{0x0F12, 0x9903},	// 700021B22
	{0x0F12, 0xF000},	// 700021B44
	{0x0F12, 0xF8B4},	// 700021B66
	{0x0F12, 0x9001},	// 700021B88
	{0x0F12, 0x8828},	// 700021BAA
	{0x0F12, 0x466B},	// 700021BCC
	{0x0F12, 0x8118},	// 700021BEE
	{0x0F12, 0x8868},	// 700021C00
	{0x0F12, 0x8158},	// 700021C22
	{0x0F12, 0x980A},	// 700021C44
	{0x0F12, 0x9902},	// 700021C66
	{0x0F12, 0xF000},	// 700021C88
	{0x0F12, 0xF8AA},	// 700021CAA
	{0x0F12, 0x8AB9},	// 700021CCC
	{0x0F12, 0x9A08},	// 700021CEE
	{0x0F12, 0x4351},	// 700021D00
	{0x0F12, 0x17CA},	// 700021D22
	{0x0F12, 0x0E12},	// 700021D44
	{0x0F12, 0x1851},	// 700021D66
	{0x0F12, 0x120A},	// 700021D88
	{0x0F12, 0x9901},	// 700021DAA
	{0x0F12, 0xF000},	// 700021DCC
	{0x0F12, 0xF890},	// 700021DEE
	{0x0F12, 0x0407},	// 700021E00
	{0x0F12, 0x0C3F},	// 700021E22
	{0x0F12, 0xE000},	// 700021E44
	{0x0F12, 0x2700},	// 700021E66
	{0x0F12, 0x8820},	// 700021E88
	{0x0F12, 0x466B},	// 700021EAA
	{0x0F12, 0xAA05},	// 700021ECC
	{0x0F12, 0x8198},	// 700021EEE
	{0x0F12, 0x8860},	// 700021F00
	{0x0F12, 0x81D8},	// 700021F22
	{0x0F12, 0x8828},	// 700021F44
	{0x0F12, 0x8118},	// 700021F66
	{0x0F12, 0x8868},	// 700021F88
	{0x0F12, 0x8158},	// 700021FAA
	{0x0F12, 0xA802},	// 700021FCC
	{0x0F12, 0xC803},	// 700021FEE
	{0x0F12, 0x003B},	// 700022000
	{0x0F12, 0xF000},	// 700022022
	{0x0F12, 0xF895},	// 700022044
	{0x0F12, 0x88A1},	// 700022066
	{0x0F12, 0x88A8},	// 700022088
	{0x0F12, 0x003A},	// 7000220AA
	{0x0F12, 0xF000},	// 7000220CC
	{0x0F12, 0xF898},	// 7000220EE
	{0x0F12, 0x0004},	// 700022100
	{0x0F12, 0xA804},	// 700022122
	{0x0F12, 0xC803},	// 700022144
	{0x0F12, 0x9A09},	// 700022166
	{0x0F12, 0x9B07},	// 700022188
	{0x0F12, 0xF000},	// 7000221AA
	{0x0F12, 0xF889},	// 7000221CC
	{0x0F12, 0xA806},	// 7000221EE
	{0x0F12, 0xC805},	// 700022200
	{0x0F12, 0x0021},	// 700022222
	{0x0F12, 0xF000},	// 700022244
	{0x0F12, 0xF88C},	// 700022266
	{0x0F12, 0x6030},	// 700022288
	{0x0F12, 0xB00B},	// 7000222AA
	{0x0F12, 0xBCF0},	// 7000222CC
	{0x0F12, 0xBC08},	// 7000222EE
	{0x0F12, 0x4718},	// 700022300
	{0x0F12, 0xB510},	// 700022322
	{0x0F12, 0x6800},	// 700022344
	{0x0F12, 0x2805},	// 700022366
	{0x0F12, 0xD11E},	// 700022388
	{0x0F12, 0x481D},	// 7000223AA
	{0x0F12, 0x491D},	// 7000223CC
	{0x0F12, 0x8800},	// 7000223EE
	{0x0F12, 0x8348},	// 700022400
	{0x0F12, 0x2301},	// 700022422
	{0x0F12, 0x31A0},	// 700022444
	{0x0F12, 0x720B},	// 700022466
	{0x0F12, 0x2000},	// 700022488
	{0x0F12, 0x000A},	// 7000224AA
	{0x0F12, 0x3A60},	// 7000224CC
	{0x0F12, 0x7150},	// 7000224EE
	{0x0F12, 0x71CB},	// 700022500
	{0x0F12, 0x22FF},	// 700022522
	{0x0F12, 0x3980},	// 700022544
	{0x0F12, 0x75CA},	// 700022566
	{0x0F12, 0x764A},	// 700022588
	{0x0F12, 0x000A},	// 7000225AA
	{0x0F12, 0x3260},	// 7000225CC
	{0x0F12, 0x7210},	// 7000225EE
	{0x0F12, 0x7250},	// 700022600
	{0x0F12, 0x2008},	// 700022622
	{0x0F12, 0x7488},	// 700022644
	{0x0F12, 0x74C8},	// 700022666
	{0x0F12, 0x4813},	// 700022688
	{0x0F12, 0x217E},	// 7000226AA
	{0x0F12, 0x43C9},	// 7000226CC
	{0x0F12, 0x8101},	// 7000226EE
	{0x0F12, 0x2306},	// 700022700
	{0x0F12, 0x5EC0},	// 700022722
	{0x0F12, 0xF000},	// 700022744
	{0x0F12, 0xF86C},	// 700022766
	{0x0F12, 0xBC10},	// 700022788
	{0x0F12, 0xBC08},	// 7000227AA
	{0x0F12, 0x4718},	// 7000227CC
	{0x0F12, 0xB570},	// 7000227EE
	{0x0F12, 0x2200},	// 700022800
	{0x0F12, 0x490E},	// 700022822
	{0x0F12, 0x480E},	// 700022844
	{0x0F12, 0x2401},	// 700022866
	{0x0F12, 0xF000},	// 700022888
	{0x0F12, 0xF86A},	// 7000228AA
	{0x0F12, 0x0022},	// 7000228CC
	{0x0F12, 0x490D},	// 7000228EE
	{0x0F12, 0x480D},	// 700022900
	{0x0F12, 0x2502},	// 700022922
	{0x0F12, 0xF000},	// 700022944
	{0x0F12, 0xF864},	// 700022966
	{0x0F12, 0x490C},	// 700022988
	{0x0F12, 0x480D},	// 7000229AA
	{0x0F12, 0x002A},	// 7000229CC
	{0x0F12, 0xF000},	// 7000229EE
	{0x0F12, 0xF85F},	// 700022A00
	{0x0F12, 0xBC70},	// 700022A22
	{0x0F12, 0xBC08},	// 700022A44
	{0x0F12, 0x4718},	// 700022A66
	{0x0F12, 0x1A28},	// 700022A88
	{0x0F12, 0x7000},	// 700022AAA
	{0x0F12, 0x0D64},	// 700022ACC
	{0x0F12, 0x7000},	// 700022AEE
	{0x0F12, 0x2370},	// 700022B00
	{0x0F12, 0x7000},	// 700022B22
	{0x0F12, 0x1C14},	// 700022B44
	{0x0F12, 0x7000},	// 700022B66
	{0x0F12, 0x14FA},	// 700022B88
	{0x0F12, 0x7000},	// 700022BAA
	{0x0F12, 0x2115},	// 700022BCC
	{0x0F12, 0x7000},	// 700022BEE
	{0x0F12, 0x622F},	// 700022C00
	{0x0F12, 0x0000},	// 700022C22
	{0x0F12, 0x208D},	// 700022C44
	{0x0F12, 0x7000},	// 700022C66
	{0x0F12, 0x6445},	// 700022C88
	{0x0F12, 0x0000},	// 700022CAA
	{0x0F12, 0x2233},	// 700022CCC
	{0x0F12, 0x7000},	// 700022CEE
	{0x0F12, 0x3B5B},	// 700022D00
	{0x0F12, 0x0000},	// 700022D22
	{0x0F12, 0x4778},	// 700022D44
	{0x0F12, 0x46C0},	// 700022D66
	{0x0F12, 0xF004},	// 700022D88
	{0x0F12, 0xE51F},	// 700022DAA
	{0x0F12, 0xA464},	// 700022DCC
	{0x0F12, 0x0000},	// 700022DEE
	{0x0F12, 0x4778},	// 700022E00
	{0x0F12, 0x46C0},	// 700022E22
	{0x0F12, 0xC000},	// 700022E44
	{0x0F12, 0xE59F},	// 700022E66
	{0x0F12, 0xFF1C},	// 700022E88
	{0x0F12, 0xE12F},	// 700022EAA
	{0x0F12, 0x6009},	// 700022ECC
	{0x0F12, 0x0000},	// 700022EEE
	{0x0F12, 0x4778},	// 700022F00
	{0x0F12, 0x46C0},	// 700022F22
	{0x0F12, 0xC000},	// 700022F44
	{0x0F12, 0xE59F},	// 700022F66
	{0x0F12, 0xFF1C},	// 700022F88
	{0x0F12, 0xE12F},	// 700022FAA
	{0x0F12, 0x622F},	// 700022FCC
	{0x0F12, 0x0000},	// 700022FEE
	{0x0F12, 0x4778},	// 700023000
	{0x0F12, 0x46C0},	// 700023022
	{0x0F12, 0xC000},	// 700023044
	{0x0F12, 0xE59F},	// 700023066
	{0x0F12, 0xFF1C},	// 700023088
	{0x0F12, 0xE12F},	// 7000230AA
	{0x0F12, 0x5F49},	// 7000230CC
	{0x0F12, 0x0000},	// 7000230EE
	{0x0F12, 0x4778},	// 700023100
	{0x0F12, 0x46C0},	// 700023122
	{0x0F12, 0xC000},	// 700023144
	{0x0F12, 0xE59F},	// 700023166
	{0x0F12, 0xFF1C},	// 700023188
	{0x0F12, 0xE12F},	// 7000231AA
	{0x0F12, 0x5FC7},	// 7000231CC
	{0x0F12, 0x0000},	// 7000231EE
	{0x0F12, 0x4778},	// 700023200
	{0x0F12, 0x46C0},	// 700023222
	{0x0F12, 0xC000},	// 700023244
	{0x0F12, 0xE59F},	// 700023266
	{0x0F12, 0xFF1C},	// 700023288
	{0x0F12, 0xE12F},	// 7000232AA
	{0x0F12, 0x5457},	// 7000232CC
	{0x0F12, 0x0000},	// 7000232EE
	{0x0F12, 0x4778},	// 700023300
	{0x0F12, 0x46C0},	// 700023322
	{0x0F12, 0xC000},	// 700023344
	{0x0F12, 0xE59F},	// 700023366
	{0x0F12, 0xFF1C},	// 700023388
	{0x0F12, 0xE12F},	// 7000233AA
	{0x0F12, 0x5FA3},	// 7000233CC
	{0x0F12, 0x0000},	// 7000233EE
	{0x0F12, 0x4778},	// 700023400
	{0x0F12, 0x46C0},	// 700023422
	{0x0F12, 0xC000},	// 700023444
	{0x0F12, 0xE59F},	// 700023466
	{0x0F12, 0xFF1C},	// 700023488
	{0x0F12, 0xE12F},	// 7000234AA
	{0x0F12, 0x51F9},	// 7000234CC
	{0x0F12, 0x0000},	// 7000234EE
	{0x0F12, 0x4778},	// 700023500
	{0x0F12, 0x46C0},	// 700023522
	{0x0F12, 0xC000},	// 700023544
	{0x0F12, 0xE59F},	// 700023566
	{0x0F12, 0xFF1C},	// 700023588
	{0x0F12, 0xE12F},	// 7000235AA
	{0x0F12, 0x7633},	// 7000235CC
	{0x0F12, 0x0000},	// 7000235EE
	{0x0F12, 0x4778},	// 700023600
	{0x0F12, 0x46C0},	// 700023622
	{0x0F12, 0xC000},	// 700023644
	{0x0F12, 0xE59F},	// 700023666
	{0x0F12, 0xFF1C},	// 700023688
	{0x0F12, 0xE12F},	// 7000236AA
	{0x0F12, 0xA007},	// 7000236CC
	{0x0F12, 0x0000},	// 7000236EE
	{0x0F12, 0x0360},	// 700023700
	{0x0F12, 0x6F4E},	// 700023722
	{0x0F12, 0x2076},	// 700023744
	{0x0F12, 0x3831},	// 700023766
	{0x0F12, 0x3220},	// 700023788
	{0x0F12, 0x3030},	// 7000237AA
	{0x0F12, 0x0038},	// 7000237CC
	{0x0F12, 0xE010},	// 7000237EE
	{0x0F12, 0x0208},	// 700023800
	{0x0F12, 0x0038},	// 700023822
	// End of Trap and Patch (Last : 70002382h) // Total Size 772 (0x0304)
	{0x0028, 0xD000},
	{0x002A, 0x1000},
	{0x0F12, 0x0001},

	{0x0028, 0x7000},
	{0x002A, 0x0ADC},
	{0x0F12, 0x0AF0},	//#setot_uOnlineClocksDiv4OnlineC0             //2800
	{0x002A, 0x0AE2},
	{0x0F12, 0x222E},	//#setot_usSetRomWaitStatesSetRomThreshold4KHz //ROM Wait Threshold to 35MHz
	{0x002A, 0x0B94},
	{0x0F12, 0x0500},	//#awbb_GainsInit_0_:R    insInit
	{0x0F12, 0x0400},	//#awbb_GainsInit_1_:G    insInit
	{0x0F12, 0x0700},	//#awbb_GainsInit_2_:B    insInit
	{0x002A, 0x04A0},
	{0x0F12, 0x8000},	//#lt_uLeiInit:AE start   Init:AE
	{0x002A, 0x049A},
	{0x0F12, 0x003B},	//#lt_uMinExp             Exp



	//=================================================================================================
	//	Set CIS/APS/Analog
	//=================================================================================================
	// This registers are for FACTORY ONLY. If you change it without prior notification,
	// YOU are RESPONSIBLE for the FAILURE that will happen in the future.
	//=================================================================================================
	{0x0028, 0xD000},
	{0x002A, 0xF106},
	{0x0F12, 0x0001},	// L-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xF206},
	{0x0F12, 0x0001},	// F-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xC202},
	{0x0F12, 0x0700},	// tgr_coarse_integration_time[15:0]: revised by Ana 080115
	{0x002A, 0xF414},
	{0x0F12, 0x0030},	// aig_adc_sat[7:0] (aig_adc_sat[7:4]+1)*33mV + 690mV
	{0x002A, 0xC204},
	{0x0F12, 0x0100},	// tgr_analogue_code_global[12:0] Analog gain X8
	{0x002A, 0xF402},
	{0x0F12, 0x0092},	// aig_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_offset2[7:0]: revised by Ana 080425
	{0x002A, 0xF700},
	{0x0F12, 0x0040},	// bpradlc_control[7:0]: revised by Ana 080529 // bpradlc_nactive_pedestal[7:5],bpradlc_passthrough[1],bpradlc_bypass[0]
	{0x002A, 0xF708},
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_r[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gr[7:0]: revised by Tg 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gb[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_b[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_total[7:0]: revised by TG 080529
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_r[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gr[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gb[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_b[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_total[7:0]: revised by Ana 080425
	{0x0F12, 0x0001},	// bpradlc_adlc_en[7:0]: revised by TG 080529	// bpradlc_f_l_adlc_en[2],bpradlc_f_adlc_en[1],bpradlc_h_adlc_en[0]
	{0x0F12, 0x0015},	// bpradlc_adlc_option[7:0]: revised by TG 080425 // bpradlc_h_adlc_ch_sel[4], // bpradlc_max_data_clip_sel[3:2],bpradlc_f_adlc_max_data_clip_sel[1:0]
	{0x0F12, 0x0001},	// bpradlc_adlc_bpr_en[7:0] adlc_bpr_enable: revised by Ana 080112
	{0x0F12, 0x0040},	// bpradlc_adlc_bpr_thresh[7:0]: 080425
	{0x002A, 0xF48A},
	{0x0F12, 0x0048},	// aig_ld_ptr7[9:0] line OB start addr(04h~48h): revised by Ana 080911
	{0x002A, 0xF10A},
	{0x0F12, 0x008B},	// tgr_h_desc0_addr_end[10:0] line OB end addr(47h~8Bh): revised by Ana 080911 // line OB start - line OB end = 67d
	{0x002A, 0xF900},
	{0x0F12, 0x0067},	// cfpn_config: revised by Ana 080425 // cfpn_ref_gain[9:7],cpfn_ref_lines[6:5],cfpn_hbinning[4],cfpn_output_direct[3],// cfpn_overflow_protect[2],cpfn_passthrough[1],cfpn_bypass[0]
	{0x002A, 0xF406},
	{0x0F12, 0x0092},	// aig_cfpn_ref_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_cfpn_ref_offset2[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_cfpn_ref_gain[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_ld_ctrl[1:0] aig_ldb_en[1], aig_ld_en[0]
	{0x0F12, 0x0000},	// aig_row_id_ctrl[1:0]
	{0x002A, 0xF442},
	{0x0F12, 0x0000},	// aig_vavg[0]
	{0x0F12, 0x0000},	// aig_havg[0]=1b @H-avg mode: revised by Ana 080116
	{0x002A, 0xF448},
	{0x0F12, 0x0000},	// aig_sl_off[0]
	{0x002A, 0xF454},
	{0x0F12, 0x0001},	//Power save enable
	{0x0F12, 0x0001},	// aig_blst_en[0]
	{0x0F12, 0x0010},	// aig_blst_en_cintr[15:0]
	{0x0F12, 0x0000},	// aig_dshut_en[0]
	{0x002A, 0xF41A},
	{0x0F12, 0x00FF},	// aig_comp_bias[7:0] aig_comp2_bias[7:4], aig_comp1_bias[3:0]: revised by Ana 081013
	{0x0F12, 0x0003},	// aig_pix_bias[3:0]
	{0x002A, 0xF420},
	{0x0F12, 0x0030},	// aig_clp_lvl[7:0]: revised by Ana 080910(refer to 6AA)
	{0x002A, 0xF410},
	{0x0F12, 0x0001},	// aig_clp_sl_ctrl[0]
	{0x0F12, 0x0000},	// aig_cds_test[7:0], aig_cds_test[1]=1b @H-avg mode current save: revised by Ana 080116
	{0x002A, 0xF416},
	{0x0F12, 0x0001},	// aig_rmp_option[7:0]
	{0x002A, 0xF424},
	{0x0F12, 0x0000},	// aig_ref_option[7:0], aig_ref_option[0]=1b @H-avg mode odd cds off: revised by Ana 080116
	{0x002A, 0xF422},
	{0x0F12, 0x0000},	// aig_monit[7:0]
	{0x002A, 0xF41E},
	{0x0F12, 0x0000},	// aig_pd_pix[0]
	{0x002A, 0xF428},
	{0x0F12, 0x0000},	// aig_pd_cp[1:0] aig_pd_ncp[1], aig_pd_cp[0]
	{0x0F12, 0x0000},	// aig_pd_reg_pix[0]
	{0x0F12, 0x0000},	// aig_pd_reg_rg[0]
	{0x002A, 0xF430},
	{0x0F12, 0x0000},	// aig_pd_reg_tgsl[0]
	{0x0F12, 0x0000},	// aig_pd_reg_ntg[0]
	{0x0F12, 0x0008},	// aig_rosc_tune_cp[3:0]: revised by Ana 080418
	{0x0F12, 0x0005},	// aig_rosc_tune_ncp[3:0]
	{0x0F12, 0x000F},	// aig_cp_capa[3:0]
	{0x0F12, 0x0001},	// aig_reg_tune_pix[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_rg[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_tgsl[7:0]
	{0x0F12, 0x0010},	// aig_reg_tune_ntg[7:0] 0040 -> 0010
	{0x002A, 0xF4D6},
	{0x0F12, 0x00C0},	// aig_v15_tune[7:0]: revised by Ana 081010 // aig_v15_tune[7:4]=7h @sensor only Mode // aig_v15_tune[7:4]=9h @ISP 7.5fps Mode // aig_v15_tune[7:4]=Dh @ISP 15fps Mode
	{0x0F12, 0x0000},	//aig_vreg_sel[7:0] [9]h_test, [1]reg_sw_stby, [0]aig_reg_sel
	{0x002A, 0xF47C},
	{0x0F12, 0x000C},	//aig_ld_ptr0[4:0]
	{0x0F12, 0x0000},	//aig_ld_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF49A},
	{0x0F12, 0x0008},	//aig_sla_ptr0[3:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_sla_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4A2},
	{0x0F12, 0x0008},	//aig_slb_ptr0[7:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_slb_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4B2},
	{0x0F12, 0x0013},	//aig_rxa_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxa_ptr1[9:0]: revised by Ana 081010
	{0x0F12, 0x0013},	//aig_rxb_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxb_ptr1[9:0]: revised by Ana 081010
	{0x002A, 0xF4AA},
	{0x0F12, 0x009B},	//aig_txa_ptr0[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txa_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_txb_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txb_ptr1[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF474},
	{0x0F12, 0x0017},	//aig_s3_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x005F},	//aig_s3_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0017},	//aig_s4_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x008F},	//aig_s4_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF48C},
	{0x0F12, 0x0017},	//aig_clp_sl_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_clp_sl_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF4C8},
	{0x0F12, 0x0163},	//aig_off_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0193},	//aig_rmp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF490},
	{0x0F12, 0x0191},	//aig_comp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF418},
	{0x0F12, 0x0083},	//aig_dbs_option[7:0]: revised by Ana 081010

	//For ESD Check
	{0x0028, 0x7000},
	{0x002A, 0x0132},
	{0x0F12, 0xAAAA},	//#define REG_FWpid (For ESD Check)
	// * note : ESD Check Method.
	//	1. if (7000.0132 != AAAA ) reset // ( S/W reg. check)
	//	2. if (D000.1002 != 0000 ) reset // (internal Exception)

	//Set FPN Gain Input
	{0x002A, 0x1176},
	{0x0F12, 0x0020},	// fpn_GainInput[0]
	{0x0F12, 0x0040},	// fpn_GainInput[1]
	{0x0F12, 0x0080},	// fpn_GainInput[2]
	{0x0F12, 0x0100},	// fpn_GainInput[3]
	{0x0F12, 0x0014},	// fpn_GainOutput[0]
	{0x0F12, 0x000A},	// fpn_GainOutput[1]
	{0x0F12, 0x0008},	// fpn_GainOutput[2]
	{0x0F12, 0x0004},	// fpn_GainOutput[3]

	// Set CFPN Canceller
	{0x002A, 0x116E},
	{0x0F12, 0x0000},	// OvflProtect
	{0x0F12, 0x0000},	// bypassThrough
	{0x0F12, 0x0000},	// bypass
	{0x0F12, 0x0002},	// fpn.CollectMethod	// 0 : Center	1 : Right	2: LEFT
	{0x002A, 0x0AE8},
	{0x0F12, 0x0000},	// setot_bSendFpnToISP = ??

	// Set Forbidden Area
	{0x002A, 0x10EE},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_usRegCount // 0 value --> not use aig table 3
	{0x002A, 0x10F2},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_pSenModesRegsArray3[0]		//real register 0xD000F45A
	{0x002A, 0x1152},
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[48]    //real register 0xD000F4BA
	{0x0F12, 0x0028},	// senHal_SenRegsModes3_pSenModesRegsArray3[49]    //real register 0xD000F4BC
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[50]    //real register 0xD000F4BE
	{0x002A, 0x1148},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[43]    //real register 0xD000F4B0
	{0x002A, 0x1144},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[41]    //real register 0xD000F4AC
	{0x002A, 0x1150},
	{0x0F12, 0x0148},	// senHal_SenRegsModes3_pSenModesRegsArray3[47]    //real register 0xD000F4B8

	// Set AE Target
	{0x002A, 0x0F4C},
	{0x0F12, 0x003D},	// #TVAR_ae_BrAve



	//=================================================================================================
	//	Set Frame Rate
	//=================================================================================================
	{0x002A, 0x0484},
	{0x0F12, 0x61A8},	// #lt_uMaxExp1 	// 50ms
	{0x0F12, 0x0000},
	{0x0F12, 0xE848},	// #lt_uMaxExp2 	// 250ms
	{0x0F12, 0x0001},
	{0x0F12, 0x03C4},	// #lt_uCapMaxExp1	// 133ms
	{0x0F12, 0x0001},
	{0x0F12, 0xE848},	// #lt_uCapMaxExp2	// 250ms
	{0x0F12, 0x0001},
	{0x0F12, 0x04C0},	// #lt_uMaxAnGain1	// x4.75
	{0x0F12, 0x0700},	// #lt_uMaxAnGain2	// x7.0
	{0x0F12, 0x0100},	// #lt_uMaxDigGain
	{0x002A, 0x0F52},
	{0x0F12, 0x0007},	// #ae_StatMode



	//=================================================================================================
	//	Set Flicker  Current Flicker = 7000.164A //Mon_AFC_usFlExpQunat		0001:Off 1045:60Hz	1388:50Hz
	//=================================================================================================

	//1. Auto Flicker 60Hz Start
	{0x002A, 0x0B2E},
	{0x0F12, 0x0001},	// #AFC_Default60Hz 	1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	{0x002A, 0x03F8},
	{0x0F12, 0x007F},	// #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////2. Auto Flicker 50Hz Start
	//WRITE	70000B2E	0000	// #AFC_Default60Hz		1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	//WRITE 700003F8	007F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////3. Manual Flicker 60Hz


	//=================================================================================================
	//	Set PLL
	//=================================================================================================
	// Input CLK
	{0x002A, 0x01B8},
	{0x0F12, 0x5DC0},	//5DC0	//#REG_TC_IPRM_InClockLSBs	//24 -> 24.5M Hz(20081208)
	{0x0F12, 0x0000},	//#REG_TC_IPRM_InClockMSBs

	// Target Sys CLK
	{0x002A, 0x01CC},
	{0x0F12, 0x1770},	// #REG_TC_IPRM_OpClk4KHz_0 			//24Mhz

	// Target PVI CLK
	{0x0F12, 0x2318},	// #REG_TC_IPRM_MinOutRate4KHz_0	//36Mhz
	{0x0F12, 0x2338},	// #REG_TC_IPRM_MaxOutRate4KHz_0

	// PLL ETC
	{0x002A, 0x01C6},
	{0x0F12, 0x0003},	//#REG_TC_IPRM_UseNPviClocks
	{0x0F12, 0x0000},	//#REG_TC_IPRM_UseNMipiClocks
	{0x002A, 0x01C0},
	{0x0F12, 0x0000},	//#EG_TC_IPRM_ValidVActiveLow
	{0x002A, 0x01DE},
	{0x0F12, 0x0001},	//#REG_TC_IPRM_UseRegsAPI
	{0x0F12, 0x0001},	//#REG_TC_IPRM_InitParamsUpdated
	{0x002A, 0x1084},
	{0x0F12, 0x0001},	//#senHal_bUseAnalogBinning //Avg S.S
	{0x002A, 0x10B6},
	{0x0F12, 0x0048},	//#senHal_uMinColsAddAnalogBin



	//=================================================================================================
	//	Set Preview Config
	//=================================================================================================

	//* Preview Cropping Code *//

	{0x002A, 0x0242},
	{0x0F12, 0x0320}, //0x0280, //0x0140(320)->640, //090416 800 -> 320, 0x0320,	                 1280 -> 0x500  //0280  // #REG_0TC_PCFG_usWidth						// 640 -> 800 _20081209
	{0x0F12, 0x0258}, //0x01e0, //0x00f0(240)->480, //090416 600 -> 240, 0x0258, 0x168 -> 360     720 -> 0x2D0	//01e0  // #REG_0TC_PCFG_usHeight						// 480 -> 600 _20081209
	{0x0F12, 0x0005},	// #REG_0TC_PCFG_Format 						// YUV output
	{0x0F12, 0x2338},	// #REG_0TC_PCFG_usMaxOut4KHzRate
	{0x0F12, 0x2318},	// #REG_0TC_PCFG_usMinOut4KHzRate
//	{0x0F12, 0x446C},	// #REG_0TC_PCFG_usMaxOut4KHzRate
//	{0x0F12, 0x444C},	// #REG_0TC_PCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	// #REG_0TC_PCFG_PVIMask   (x2: PCLK rising x0: PCLK Falling)
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_uClockInd
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_usFrTimeType
//	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType		(0 : bin mode, 2 : No-bin mode)
	{0x0F12, 0x02AF},	//03E8},	// #REG_0TC_PCFG_usMaxFrTimeMsecMult10 // 10fps --> 15fps
	{0x0F12, 0x01F4},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 20fps
//	{0x0F12, 0x014D},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 30fps
	{0x002A, 0x0262},
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uPrevMirror -- X/Y flip 20090112
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uCaptureMirror -- X/Y flip 20090112
	//WRITE 70000262 000F  //#REG_0TC_PCFG_uPrevMirror
	//WRITE 70000264 000F  //#REG_0TC_PCFG_uCaptureMirror

	// Apply preview config
	{0x002A, 0x021C},
	{0x0F12, 0x0000},	//#REG_TC_GP_ActivePrevConfig	  //preview config0
	{0x002A, 0x0220},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevOpenAfterChange  //config 변경적용
	{0x002A, 0x01F8},
	{0x0F12, 0x0001},	//#REG_TC_GP_NewConfigSync
	{0x002A, 0x021E},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevConfigChanged

	//=================================================================================================
	//	Set Capture Config
	//=================================================================================================
	//WRITE 70000214	04C0	// #REG_TC_GP_CapZoomReqInputHeight	// Vsize = 1216
	//WRITE 70000216	0000	// #REG_TC_GP_CapZoomReqInputWidthOfs
	//WRITE 70000218	0000	// #REG_TC_GP_CapZoomReqInputHeightOfs
	//WRITE 7000021A	0001	// #REG_TC_GP_InputsChangeRequest
	//WRITE 700001F8	0001	// #REG_TC_GP_NewConfigSync
	//

	{0x002A, 0x0302},
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uCaptureMode
	{0x0F12, 0x0640},	//#REG_0TC_CCFG_usWidth
	{0x0F12, 0x04B0},	//#REG_0TC_CCFG_usHeight						// Vsize = 1216
	{0x0F12, 0x0005},	//#REG_0TC_CCFG_Format	   //5:YUV,9:JPEG
	{0x0F12, 0x2338},	//#REG_0TC_CCFG_usMaxOut4KHzRate
	{0x0F12, 0x2318},	//#REG_0TC_CCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	//#REG_0TC_CCFG_PVIMask (x2: PCLK rising	x0: PCLK Falling)
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uClockInd
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_usFrTimeType
	{0x0F12, 0x0002},	//#REG_0TC_CCFG_FrRateQualityType     (0 : bin mode, 2 : No-bin mode)
	{0x0F12, 0x09C4},	//07D0},	//#REG_0TC_CCFG_usMaxFrTimeMsecMult10 // 5fps --> 4fps
	{0x0F12, 0x0535},	//#REG_0TC_CCFG_usMinFrTimeMsecMult10 // 7.5fps


	//* Capture Configuration Update *//
	{0x002A, 0x01F8},
	{0x0F12, 0x0001}, //REG_TC_GP_NewConfigSync 0001 : update configuration
	{0x002a, 0x0226},
	{0x0f12, 0x0001}, //REG_TC_GP_CapConfigChanged 0001

	//=================================================================================================
	//	Set Periodic Mismatch (Use default value)
	//=================================================================================================



	//=================================================================================================
	//	Set AE Weights
	//=================================================================================================
	{0x002A, 0x0F5A},
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_0_
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_1_
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_2_
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_3_
	{0x0F12, 0x0201},	//0101	// #ae_WeightTbl_16_4_
	{0x0F12, 0x0302},	//0201	// #ae_WeightTbl_16_5_
	{0x0F12, 0x0203},	//0102	// #ae_WeightTbl_16_6_
	{0x0F12, 0x0102},	//0101	// #ae_WeightTbl_16_7_
	{0x0F12, 0x0201},	//0101	// #ae_WeightTbl_16_8_
	{0x0F12, 0x0503},	//0202	// #ae_WeightTbl_16_9_
	{0x0F12, 0x0305},	//0202	// #ae_WeightTbl_16_10
	{0x0F12, 0x0102},	//0101	// #ae_WeightTbl_16_11
	{0x0F12, 0x0301},	//0101	// #ae_WeightTbl_16_12
	{0x0F12, 0x0505},	//0402	// #ae_WeightTbl_16_13
	{0x0F12, 0x0505},	//0204	// #ae_WeightTbl_16_14
	{0x0F12, 0x0103},	//0101	// #ae_WeightTbl_16_15
	{0x0F12, 0x0301},	//0201	// #ae_WeightTbl_16_16
	{0x0F12, 0x0505},	//0504	// #ae_WeightTbl_16_17
	{0x0F12, 0x0505},	//0405	// #ae_WeightTbl_16_18
	{0x0F12, 0x0103},	//0102	// #ae_WeightTbl_16_19
	{0x0F12, 0x0201},	//0501	// #ae_WeightTbl_16_20
	{0x0F12, 0x0503},	//0505	// #ae_WeightTbl_16_21
	{0x0F12, 0x0305},	//0505	// #ae_WeightTbl_16_22
	{0x0F12, 0x0102},	//0105	// #ae_WeightTbl_16_23
	{0x0F12, 0x0201},	//0503	// #ae_WeightTbl_16_24
	{0x0F12, 0x0302},	//0505	// #ae_WeightTbl_16_25
	{0x0F12, 0x0203},	//0505	// #ae_WeightTbl_16_26
	{0x0F12, 0x0102},	//0305	// #ae_WeightTbl_16_27
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_28
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_29
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_30
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_31



	//=================================================================================================
	//	Set GAS & CCM White Point
	//=================================================================================================
	// param_start	TVAR_ash_AwbAshCord
	{0x002A, 0x0704},
	{0x0F12, 0x0090},	//000E	// #TVAR_ash_AwbAshCord_0_
	{0x0F12, 0x00B3},	//0012	// #TVAR_ash_AwbAshCord_1_
	{0x0F12, 0x00C3},	//0019	// #TVAR_ash_AwbAshCord_2_
	{0x0F12, 0x00E5},	//0034	// #TVAR_ash_AwbAshCord_3_
	{0x0F12, 0x0116},	//0044	// #TVAR_ash_AwbAshCord_4_
	{0x0F12, 0x0198},	//0052	// #TVAR_ash_AwbAshCord_5_
	{0x0F12, 0x0200},	//0058	// #TVAR_ash_AwbAshCord_6_

	// param_start	wbt_AwbCcmCord
	{0x002A, 0x06F2},
	{0x0F12, 0x00B3},	//0016	// #SARR_AwbCcmCord_0_	Hor
	{0x0F12, 0x00E5},	//001E	// #SARR_AwbCcmCord_1_	IncaA
	{0x0F12, 0x0100},	//0024	// #SARR_AwbCcmCord_2_	WW
	{0x0F12, 0x0116},	//0038	// #SARR_AwbCcmCord_3_	CW
	{0x0F12, 0x0150},	//004C	// #SARR_AwbCcmCord_4_	D50
	{0x0F12, 0x0198},	//0058	// #SARR_AwbCcmCord_5_	D65

	// Set Target Brightness Control
	{0x002A, 0x103E},
	{0x0F12, 0x0000},	// #SARR_IllumType_0_
	{0x0F12, 0x0009},	// #SARR_IllumType_1_
	{0x0F12, 0x0018},	// #SARR_IllumType_2_
	{0x0F12, 0x0032},	// #SARR_IllumType_3_
	{0x0F12, 0x004A},	// #SARR_IllumType_4_
	{0x0F12, 0x0051},	// #SARR_IllumType_5_
	{0x0F12, 0x0056},	// #SARR_IllumType_6_
	{0x0F12, 0x010C},	// #SARe_2_R_IllumTypeF_0_
	{0x0F12, 0x010C},	// #SARe_3_R_IllumTypeF_1_
	{0x0F12, 0x0109},	// #SARe_4_R_IllumTypeF_2_
	{0x0F12, 0x0105},	// #SARe_5_R_IllumTypeF_3_
	{0x0F12, 0x0102},	// #SARe_6_R_IllumTypeF_4_
	{0x0F12, 0x00FB},	// #SARR_IllumTypeF_5_
	{0x0F12, 0x00F8},	// #SARR_IllumTypeF_6_

	// Indoor GAS Alpha
	{0x002A, 0x0712},
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[0]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[1]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[2]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[3]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[4]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[5]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[6]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[7]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[8]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[9]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[10]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[11]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[12]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[13]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[14]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[15]
	{0x0F12, 0x00E0},	// #TVAR_ash_GASAlpha[16]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[17]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[18]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[19]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[20]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[21]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[22]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[23]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[24]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[25]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[26]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[27]

	// Outdoor GAS Alpha
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_0_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_1_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_2_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_3_

	// GAS LUT Start Address
	{0x002A, 0x0754},
	{0x0F12, 0x2388},	//#TVAR_ash_pGAS
	{0x0F12, 0x7000},	//#TVAR_ash_pGAS

	// GAS LUT
	//							//1030	//1027
	{0x002A, 0x2388},
	{0x0F12, 0x01C9},	//#TVAR_ash_pGAS[0]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[1]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[2]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[3]
	{0x0F12, 0x0105},	//#TVAR_ash_pGAS[4]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[5]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[6]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[7]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[8]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[9]
	{0x0F12, 0x0160},	//#TVAR_ash_pGAS[10]
	{0x0F12, 0x0189},	//#TVAR_ash_pGAS[11]
	{0x0F12, 0x01D0},	//#TVAR_ash_pGAS[12]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[13]
	{0x0F12, 0x016A},	//#TVAR_ash_pGAS[14]
	{0x0F12, 0x0126},	//#TVAR_ash_pGAS[15]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[16]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[17]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[18]
	{0x0F12, 0x009B},	//#TVAR_ash_pGAS[19]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[20]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[21]
	{0x0F12, 0x00EF},	//#TVAR_ash_pGAS[22]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[23]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[24]
	{0x0F12, 0x019C},	//#TVAR_ash_pGAS[25]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[26]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[27]
	{0x0F12, 0x00E2},	//#TVAR_ash_pGAS[28]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[29]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[30]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[31]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[32]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[33]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[34]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[35]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[36]
	{0x0F12, 0x0133},	//#TVAR_ash_pGAS[37]
	{0x0F12, 0x016E},	//#TVAR_ash_pGAS[38]
	{0x0F12, 0x0140},	//#TVAR_ash_pGAS[39]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[40]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[41]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[42]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[43]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[44]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[45]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[46]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[47]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[48]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[49]
	{0x0F12, 0x010D},	//#TVAR_ash_pGAS[50]
	{0x0F12, 0x0157},	//#TVAR_ash_pGAS[51]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[52]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[53]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[54]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[55]
	{0x0F12, 0x002F},	//#TVAR_ash_pGAS[56]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[57]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[58]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[59]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[60]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[61]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[62]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[63]
	{0x0F12, 0x0142},	//#TVAR_ash_pGAS[64]
	{0x0F12, 0x011A},	//#TVAR_ash_pGAS[65]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[66]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[67]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[68]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[69]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[70]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[71]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[72]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[73]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[74]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[75]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[76]
	{0x0F12, 0x013B},	//#TVAR_ash_pGAS[77]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[78]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[79]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[80]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[81]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[82]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[83]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[84]
	{0x0F12, 0x0014},	//#TVAR_ash_pGAS[85]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[86]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[87]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[88]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[89]
	{0x0F12, 0x013F},	//#TVAR_ash_pGAS[90]
	{0x0F12, 0x012C},	//#TVAR_ash_pGAS[91]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[92]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[93]
	{0x0F12, 0x0067},	//#TVAR_ash_pGAS[94]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[95]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[96]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[97]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[98]
	{0x0F12, 0x004B},	//#TVAR_ash_pGAS[99]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[100]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[101]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[102]
	{0x0F12, 0x0150},	//#TVAR_ash_pGAS[103]
	{0x0F12, 0x0149},	//#TVAR_ash_pGAS[104]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[105]
	{0x0F12, 0x00C5},	//#TVAR_ash_pGAS[106]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[107]
	{0x0F12, 0x0061},	//#TVAR_ash_pGAS[108]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[109]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[110]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[111]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[112]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[113]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[114]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[115]
	{0x0F12, 0x016D},	//#TVAR_ash_pGAS[116]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[117]
	{0x0F12, 0x013A},	//#TVAR_ash_pGAS[118]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[119]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[120]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[121]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[122]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[123]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[124]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[125]
	{0x0F12, 0x00D8},	//#TVAR_ash_pGAS[126]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[127]
	{0x0F12, 0x0154},	//#TVAR_ash_pGAS[128]
	{0x0F12, 0x0186},	//#TVAR_ash_pGAS[129]
	{0x0F12, 0x018F},	//#TVAR_ash_pGAS[130]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[131]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[132]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[133]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[134]
	{0x0F12, 0x00B4},	//#TVAR_ash_pGAS[135]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[136]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[137]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[138]
	{0x0F12, 0x0112},	//#TVAR_ash_pGAS[139]
	{0x0F12, 0x014F},	//#TVAR_ash_pGAS[140]
	{0x0F12, 0x0185},	//#TVAR_ash_pGAS[141]
	{0x0F12, 0x01A1},	//#TVAR_ash_pGAS[142]
	{0x0F12, 0x0168},	//#TVAR_ash_pGAS[143]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[144]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[145]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[146]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[147]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[148]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[149]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[150]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[151]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[152]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[153]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[154]
	{0x0F12, 0x0163},	//#TVAR_ash_pGAS[155]
	{0x0F12, 0x0131},	//#TVAR_ash_pGAS[156]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[157]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[158]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[159]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[160]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[161]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[162]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[163]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[164]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[165]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[166]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[167]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[168]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[169]
	{0x0F12, 0x00D2},	//#TVAR_ash_pGAS[170]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[171]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[172]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[173]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[174]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[175]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[176]
	{0x0F12, 0x0059},	//#TVAR_ash_pGAS[177]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[178]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[179]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[180]
	{0x0F12, 0x010C},	//#TVAR_ash_pGAS[181]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[182]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[183]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[184]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[185]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[186]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[187]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[188]
	{0x0F12, 0x0025},	//#TVAR_ash_pGAS[189]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[190]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[191]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[192]
	{0x0F12, 0x00C2},	//#TVAR_ash_pGAS[193]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[194]
	{0x0F12, 0x00DD},	//#TVAR_ash_pGAS[195]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[196]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[197]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[198]
	{0x0F12, 0x001E},	//#TVAR_ash_pGAS[199]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[200]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[201]
	{0x0F12, 0x0010},	//#TVAR_ash_pGAS[202]
	{0x0F12, 0x0026},	//#TVAR_ash_pGAS[203]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[204]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[205]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[206]
	{0x0F12, 0x00EC},	//#TVAR_ash_pGAS[207]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[208]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[209]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[210]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[211]
	{0x0F12, 0x0017},	//#TVAR_ash_pGAS[212]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[213]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[214]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[215]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[216]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[217]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[218]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[219]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[220]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[221]
	{0x0F12, 0x009A},	//#TVAR_ash_pGAS[222]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[223]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[224]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[225]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[226]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[227]
	{0x0F12, 0x000F},	//#TVAR_ash_pGAS[228]
	{0x0F12, 0x0027},	//#TVAR_ash_pGAS[229]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[230]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[231]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[232]
	{0x0F12, 0x00F1},	//#TVAR_ash_pGAS[233]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[234]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[235]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[236]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[237]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[238]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[239]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[240]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[241]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[242]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[243]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[244]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[245]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[246]
	{0x0F12, 0x00FA},	//#TVAR_ash_pGAS[247]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[248]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[249]
	{0x0F12, 0x0064},	//#TVAR_ash_pGAS[250]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[251]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[252]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[253]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[254]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[255]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[256]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[257]
	{0x0F12, 0x00DF},	//#TVAR_ash_pGAS[258]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[259]
	{0x0F12, 0x0110},	//#TVAR_ash_pGAS[260]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[261]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[262]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[263]
	{0x0F12, 0x006E},	//#TVAR_ash_pGAS[264]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[265]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[266]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[267]
	{0x0F12, 0x0083},	//#TVAR_ash_pGAS[268]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[269]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[270]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[271]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[272]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[273]
	{0x0F12, 0x0104},	//#TVAR_ash_pGAS[274]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[275]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[276]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[277]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[278]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[279]
	{0x0F12, 0x0098},	//#TVAR_ash_pGAS[280]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[281]
	{0x0F12, 0x00D1},	//#TVAR_ash_pGAS[282]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[283]
	{0x0F12, 0x0127},	//#TVAR_ash_pGAS[284]
	{0x0F12, 0x0146},	//#TVAR_ash_pGAS[285]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[286]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[287]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[288]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[289]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[290]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[291]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[292]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[293]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[294]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[295]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[296]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[297]
	{0x0F12, 0x0180},	//#TVAR_ash_pGAS[298]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[299]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[300]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[301]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[302]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[303]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[304]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[305]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[306]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[307]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[308]
	{0x0F12, 0x00EE},	//#TVAR_ash_pGAS[309]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[310]
	{0x0F12, 0x014C},	//#TVAR_ash_pGAS[311]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[312]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[313]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[314]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[315]
	{0x0F12, 0x0057},	//#TVAR_ash_pGAS[316]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[317]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[318]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[319]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[320]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[321]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[322]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[323]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[324]
	{0x0F12, 0x00F6},	//#TVAR_ash_pGAS[325]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[326]
	{0x0F12, 0x0081},	//#TVAR_ash_pGAS[327]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[328]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[329]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[330]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[331]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[332]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[333]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[334]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[335]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[336]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[337]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[338]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[339]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[340]
	{0x0F12, 0x0041},	//#TVAR_ash_pGAS[341]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[342]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[343]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[344]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[345]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[346]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[347]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[348]
	{0x0F12, 0x00BA},	//#TVAR_ash_pGAS[349]
	{0x0F12, 0x00F7},	//#TVAR_ash_pGAS[350]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[351]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[352]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[353]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[354]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[355]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[356]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[357]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[358]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[359]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[360]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[361]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[362]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[363]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[364]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[365]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[366]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[367]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[368]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[369]
	{0x0F12, 0x0002},	//#TVAR_ash_pGAS[370]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[371]
	{0x0F12, 0x0023},	//#TVAR_ash_pGAS[372]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[373]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[374]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[375]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[376]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[377]
	{0x0F12, 0x00B0},	//#TVAR_ash_pGAS[378]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[379]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[380]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[381]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[382]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[383]
	{0x0F12, 0x001C},	//#TVAR_ash_pGAS[384]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[385]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[386]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[387]
	{0x0F12, 0x00B7},	//#TVAR_ash_pGAS[388]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[389]
	{0x0F12, 0x0106},	//#TVAR_ash_pGAS[390]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[391]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[392]
	{0x0F12, 0x006C},	//#TVAR_ash_pGAS[393]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[394]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[395]
	{0x0F12, 0x0033},	//#TVAR_ash_pGAS[396]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[397]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[398]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[399]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[400]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[401]
	{0x0F12, 0x0101},	//#TVAR_ash_pGAS[402]
	{0x0F12, 0x0120},	//#TVAR_ash_pGAS[403]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[404]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[405]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[406]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[407]
	{0x0F12, 0x0062},	//#TVAR_ash_pGAS[408]
	{0x0F12, 0x005C},	//#TVAR_ash_pGAS[409]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[410]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[411]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[412]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[413]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[414]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[415]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[416]
	{0x0F12, 0x0115},	//#TVAR_ash_pGAS[417]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[418]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[419]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[420]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[421]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[422]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[423]
	{0x0F12, 0x00A4},	//#TVAR_ash_pGAS[424]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[425]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[426]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[427]
	{0x0F12, 0x0130},	//#TVAR_ash_pGAS[428]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[429]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[430]
	{0x0F12, 0x00C6},	//#TVAR_ash_pGAS[431]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[432]
	{0x0F12, 0x0090},	//#TVAR_ash_pGAS[433]
	{0x0F12, 0x0085},	//#TVAR_ash_pGAS[434]
	{0x0F12, 0x0082},	//#TVAR_ash_pGAS[435]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[436]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[437]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[438]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[439]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[440]
	{0x0F12, 0x0135},	//#TVAR_ash_pGAS[441]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[442]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[443]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[444]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[445]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[446]
	{0x0F12, 0x005D},	//#TVAR_ash_pGAS[447]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[448]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[449]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[450]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[451]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[452]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[453]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[454]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[455]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[456]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[457]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[458]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[459]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[460]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[461]
	{0x0F12, 0x003D},	//#TVAR_ash_pGAS[462]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[463]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[464]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[465]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[466]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[467]
	{0x0F12, 0x00B5},	//#TVAR_ash_pGAS[468]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[469]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[470]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[471]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[472]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[473]
	{0x0F12, 0x001A},	//#TVAR_ash_pGAS[474]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[475]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[476]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[477]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[478]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[479]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[480]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[481]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[482]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[483]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[484]
	{0x0F12, 0x0018},	//#TVAR_ash_pGAS[485]
	{0x0F12, 0x000A},	//#TVAR_ash_pGAS[486]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[487]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[488]
	{0x0F12, 0x001F},	//#TVAR_ash_pGAS[489]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[490]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[491]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[492]
	{0x0F12, 0x00B6},	//#TVAR_ash_pGAS[493]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[494]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[495]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[496]
	{0x0F12, 0x0028},	//#TVAR_ash_pGAS[497]
	{0x0F12, 0x0012},	//#TVAR_ash_pGAS[498]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[499]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[500]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[501]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[502]
	{0x0F12, 0x0030},	//#TVAR_ash_pGAS[503]
	{0x0F12, 0x0050},	//#TVAR_ash_pGAS[504]
	{0x0F12, 0x007B},	//#TVAR_ash_pGAS[505]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[506]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[507]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[508]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[509]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[510]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[511]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[512]
	{0x0F12, 0x0001},	//#TVAR_ash_pGAS[513]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[514]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[515]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[516]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[517]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[518]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[519]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[520]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[521]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[522]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[523]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[524]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[525]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[526]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[527]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[528]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[529]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[530]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[531]
	{0x0F12, 0x00AE},	//#TVAR_ash_pGAS[532]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[533]
	{0x0F12, 0x0097},	//#TVAR_ash_pGAS[534]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[535]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[536]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[537]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[538]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[539]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[540]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[541]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[542]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[543]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[544]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[545]
	{0x0F12, 0x00D3},	//#TVAR_ash_pGAS[546]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[547]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[548]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[549]
	{0x0F12, 0x0058},	//#TVAR_ash_pGAS[550]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[551]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[552]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[553]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[554]
	{0x0F12, 0x006D},	//#TVAR_ash_pGAS[555]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[556]
	{0x0F12, 0x00AC},	//#TVAR_ash_pGAS[557]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[558]
	{0x0F12, 0x00F5},	//#TVAR_ash_pGAS[559]
	{0x0F12, 0x00D4},	//#TVAR_ash_pGAS[560]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[561]
	{0x0F12, 0x008E},	//#TVAR_ash_pGAS[562]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[563]
	{0x0F12, 0x0070},	//#TVAR_ash_pGAS[564]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[565]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[566]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[567]
	{0x0F12, 0x0093},	//#TVAR_ash_pGAS[568]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[569]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[570]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[571]



	//=================================================================================================
	//	Set Gamma LUT
	//=================================================================================================
	//							//New	//Hong	//ORG
	{0x002A, 0x04CC},
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[0][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[0][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[0][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[0][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[0][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[0][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[0][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[0][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[0][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[0][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[0][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[0][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[0][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[0][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[0][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[0][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[1][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[1][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[1][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[1][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[1][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[1][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[1][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[1][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[1][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[1][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[1][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[1][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[1][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[1][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[1][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[1][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[2][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[2][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[2][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[2][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[2][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[2][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[2][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[2][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[2][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[2][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[2][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[2][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[2][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[2][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[2][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[2][15]



	//=================================================================================================
	//	Set AWB
	//=================================================================================================
	{0x002A, 0x0DA8},
	{0x0F12, 0x0020},	// #awbb_LowBr0_NBzone	// 2008.04.24. eliminate to the purple color at low light.
	{0x002A, 0x0E8C},
	{0x0F12, 0x0000},	// #awbb_LowBr0_PatchNumZone
	{0x002A, 0x0D6C},
	{0x0F12, 0x0040},	// #awbb_YMedMoveToYAv

	// Indoor Gray Zone
	{0x002A, 0x0B9C},
	{0x0F12, 0x0339},	//0392	// #awbb_IndoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x0393},	//03A4	// #awbb_IndoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x031B},	//036F	// #awbb_IndoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x039C},	//03AE	// #awbb_IndoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x030A},	//034E	// #awbb_IndoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0398},	//03A2	// #awbb_IndoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x02F3},	//0331	// #awbb_IndoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0392},	//0392	// #awbb_IndoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x02E0},	//0315	// #awbb_IndoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0388},	//0381	// #awbb_IndoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x02CB},	//02F8	// #awbb_IndoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0374},	//036C	// #awbb_IndoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x02BB},	//02E1	// #awbb_IndoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x034B},	//0358	// #awbb_IndoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x02AA},	//02C5	// #awbb_IndoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x0336},	//033B	// #awbb_IndoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x029C},	//02B1	// #awbb_IndoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0321},	//0323	// #awbb_IndoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x028B},	//029C	// #awbb_IndoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x030B},	//030F	// #awbb_IndoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x027B},	//028B	// #awbb_IndoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02FD},	//02FA	// #awbb_IndoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x0268},	//027C	// #awbb_IndoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x02EA},	//02E2	// #awbb_IndoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x0259},	//0268	// #awbb_IndoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x02D8},	//02D1	// #awbb_IndoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x024C},	//0259	// #awbb_IndoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x02CB},	//02BF	// #awbb_IndoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x023D},	//024C	// #awbb_IndoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x02BD},	//02B1	// #awbb_IndoorGrZones_m_BGrid_14__m_right
	{0x0F12, 0x0231},	//023D	// #awbb_IndoorGrZones_m_BGrid_15__m_left
	{0x0F12, 0x02B1},	//02A2	// #awbb_IndoorGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0222},	//0231	// #awbb_IndoorGrZones_m_BGrid_16__m_left
	{0x0F12, 0x02A8},	//0299	// #awbb_IndoorGrZones_m_BGrid_16__m_right
	{0x0F12, 0x0215},	//0222	// #awbb_IndoorGrZones_m_BGrid_17__m_left
	{0x0F12, 0x029B},	//0290	// #awbb_IndoorGrZones_m_BGrid_17__m_right
	{0x0F12, 0x0208},	//0215	// #awbb_IndoorGrZones_m_BGrid_18__m_left
	{0x0F12, 0x0290},	//0288	// #awbb_IndoorGrZones_m_BGrid_18__m_right
	{0x0F12, 0x01F7},	//0208	// #awbb_IndoorGrZones_m_BGrid_19__m_left
	{0x0F12, 0x0287},	//0281	// #awbb_IndoorGrZones_m_BGrid_19__m_right
	{0x0F12, 0x01E6},	//01F7	// #awbb_IndoorGrZones_m_BGrid_20__m_left
	{0x0F12, 0x027F},	//0279	// #awbb_IndoorGrZones_m_BGrid_20__m_right
	{0x0F12, 0x01D9},	//01E6	// #awbb_IndoorGrZones_m_BGrid_21__m_left
	{0x0F12, 0x0279},	//026E	// #awbb_IndoorGrZones_m_BGrid_21__m_right
	{0x0F12, 0x01CC},	//01D9	// #awbb_IndoorGrZones_m_BGrid_22__m_left
	{0x0F12, 0x0272},	//0268	// #awbb_IndoorGrZones_m_BGrid_22__m_right
	{0x0F12, 0x01C1},	//01CC	// #awbb_IndoorGrZones_m_BGrid_23__m_left
	{0x0F12, 0x026C},	//0263	// #awbb_IndoorGrZones_m_BGrid_23__m_right
	{0x0F12, 0x01B3},	//01C1	// #awbb_IndoorGrZones_m_BGrid_24__m_left
	{0x0F12, 0x0265},	//0260	// #awbb_IndoorGrZones_m_BGrid_24__m_right
	{0x0F12, 0x01AA},	//01B3	// #awbb_IndoorGrZones_m_BGrid_25__m_left
	{0x0F12, 0x025C},	//0259	// #awbb_IndoorGrZones_m_BGrid_25__m_right
	{0x0F12, 0x019F},	//01AA	// #awbb_IndoorGrZones_m_BGrid_26__m_left
	{0x0F12, 0x0250},	//0253	// #awbb_IndoorGrZones_m_BGrid_26__m_right
	{0x0F12, 0x019B},	//019F	// #awbb_IndoorGrZones_m_BGrid_27__m_left
	{0x0F12, 0x0247},	//0248	// #awbb_IndoorGrZones_m_BGrid_27__m_right
	{0x0F12, 0x0197},	//019B	// #awbb_IndoorGrZones_m_BGrid_28__m_left
	{0x0F12, 0x0239},	//023F	// #awbb_IndoorGrZones_m_BGrid_28__m_right
	{0x0F12, 0x019B},	//0197	// #awbb_IndoorGrZones_m_BGrid_29__m_left
	{0x0F12, 0x021D},	//0239	// #awbb_IndoorGrZones_m_BGrid_29__m_right
	{0x0F12, 0x019D},	//019B	// #awbb_IndoorGrZones_m_BGrid_30__m_left
	{0x0F12, 0x0202},	//021D	// #awbb_IndoorGrZones_m_BGrid_30__m_right
	{0x0F12, 0x01AC},	//019D	// #awbb_IndoorGrZones_m_BGrid_31__m_left
	{0x0F12, 0x01D3},	//020A	// #awbb_IndoorGrZones_m_BGrid_31__m_right
	{0x0F12, 0x0000},	//01AC	// #awbb_IndoorGrZones_m_BGrid_32__m_left
	{0x0F12, 0x0000},	//01D3	// #awbb_IndoorGrZones_m_BGrid_32__m_right
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_left
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_right

	{0x002A, 0x0C3C},
	{0x0F12, 0x0004},	// #awbb_IndoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0138},	// #awbb_IndoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x0F12, 0x0000},

	// Outdoor Gray Zone
	{0x0F12, 0x0255},	//#awbb_OutdoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x0236},	//#awbb_OutdoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x0290},	//#awbb_OutdoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x0227},	//#awbb_OutdoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0297},	//#awbb_OutdoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x0210},	//#awbb_OutdoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0293},	//#awbb_OutdoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0200},	//#awbb_OutdoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x028B},	//#awbb_OutdoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x01F1},	//#awbb_OutdoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0286},	//#awbb_OutdoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x0282},	//#awbb_OutdoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x01E0},	//#awbb_OutdoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x027B},	//#awbb_OutdoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x01DE},	//#awbb_OutdoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0274},	//#awbb_OutdoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01DF},	//#awbb_OutdoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x0263},	//#awbb_OutdoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01F6},	//#awbb_OutdoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x0253},	//#awbb_OutdoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x020B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x023B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_right

	{0x002A, 0x0CB8},
	{0x0F12, 0x0004},	// #awbb_OutdoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x000C},	//#awbb_OutdoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0217},	//#awbb_OutdoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high
	{0x0F12, 0x0000},

	// Low Brightness Gray Zone
	{0x0F12, 0x0324},	//031E  // #awbb_LowBrGrZones_m_BGrid_0__m_left
	{0x0F12, 0x03A1},	//037E  // #awbb_LowBrGrZones_m_BGrid_0__m_right
	{0x0F12, 0x02F2},	//02CF  // #awbb_LowBrGrZones_m_BGrid_1__m_left
	{0x0F12, 0x03BA},	//0389  // #awbb_LowBrGrZones_m_BGrid_1__m_right
	{0x0F12, 0x02C4},	//02A6  // #awbb_LowBrGrZones_m_BGrid_2__m_left
	{0x0F12, 0x03B6},	//0386  // #awbb_LowBrGrZones_m_BGrid_2__m_right
	{0x0F12, 0x029E},	//0282  // #awbb_LowBrGrZones_m_BGrid_3__m_left
	{0x0F12, 0x03A6},	//0374  // #awbb_LowBrGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0283},	//0259  // #awbb_LowBrGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0380},	//0352  // #awbb_LowBrGrZones_m_BGrid_4__m_right
	{0x0F12, 0x025F},	//0239  // #awbb_LowBrGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0344},	//0330  // #awbb_LowBrGrZones_m_BGrid_5__m_right
	{0x0F12, 0x0241},	//021E  // #awbb_LowBrGrZones_m_BGrid_6__m_left
	{0x0F12, 0x031F},	//031B  // #awbb_LowBrGrZones_m_BGrid_6__m_right
	{0x0F12, 0x0227},	//020B  // #awbb_LowBrGrZones_m_BGrid_7__m_left
	{0x0F12, 0x02FC},	//0300  // #awbb_LowBrGrZones_m_BGrid_7__m_right
	{0x0F12, 0x020A},	//01F1  // #awbb_LowBrGrZones_m_BGrid_8__m_left
	{0x0F12, 0x02D9},	//02DD  // #awbb_LowBrGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01F1},	//01E0  // #awbb_LowBrGrZones_m_BGrid_9__m_left
	{0x0F12, 0x02C4},	//02C4  // #awbb_LowBrGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01D0},	//01CD  // #awbb_LowBrGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02B2},	//02AF  // #awbb_LowBrGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01B8},	//019E  // #awbb_LowBrGrZones_m_BGrid_11__m_left
	{0x0F12, 0x029E},	//02A0  // #awbb_LowBrGrZones_m_BGrid_11__m_right
	{0x0F12, 0x019F},	//0187  // #awbb_LowBrGrZones_m_BGrid_12__m_left
	{0x0F12, 0x0290},	//028C  // #awbb_LowBrGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0189},	//017D  // #awbb_LowBrGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0282},	//027E  // #awbb_LowBrGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0182},	//0179  // #awbb_LowBrGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0271},	//0258  // #awbb_LowBrGrZones_m_BGrid_14__m_right
	{0x0F12, 0x017F},	//0186  // #awbb_LowBrGrZones_m_BGrid_15__m_left
	{0x0F12, 0x0254},	//0232  // #awbb_LowBrGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0183},	//01BD  // #awbb_LowBrGrZones_m_BGrid_16__m_left
	{0x0F12, 0x022D},	//01FB  // #awbb_LowBrGrZones_m_BGrid_16__m_right
	{0x0F12, 0x019C},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_left
	{0x0F12, 0x01D3},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_right

	{0x002A, 0x0D34},
	{0x0F12, 0x0005},	// #awbb_LowBrGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0012},	// #awbb_LowBrGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0120},	// #awbb_LowBrGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0002},	// #awbb_LowBrGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D48},
	{0x0F12, 0x00E0},	// #awbb_LowBrGrZones_y_high
	{0x0F12, 0x0000},

	// Lowtemp Circle
	{0x0F12, 0x032F},	// #awbb_CrclLowT_R_c
	{0x0F12, 0x0000},
	{0x0F12, 0x017A},	// #awbb_CrclLowT_B_c
	{0x0F12, 0x0000},
	{0x0F12, 0x7300},	// #awbb_CrclLowT_Rad_c
	{0x0F12, 0x0000},
	{0x0F12, 0x000A},	// #awbb_CrclLowT_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D60},
	{0x0F12, 0x00E0},	// #awbb_CrclLowT_y_high
	{0x0F12, 0x0000},
	{0x002A, 0x0D82},
	{0x0F12, 0x0001},	// #awbb_ByPass_LowTempMode

	// White Locus
	{0x002A, 0x0D66},
	{0x0F12, 0x011F},	// #awbb_IntcR
	{0x0F12, 0x0132},	// #awbb_IntcB
	{0x002A, 0x0D74},
	{0x0F12, 0x052A},	// #awbb_MvEq_RBthresh

	// Gamut Thresholds
	{0x002A, 0x0DAE},
	{0x0F12, 0x0036},	// #awbb_GamutWidthThr2
	{0x0F12, 0x001C},	// #awbb_GamutHeightThr2
	{0x002A, 0x0DAA},
	{0x0F12, 0x071A},	// #awbb_GamutWidthThr1
	{0x0F12, 0x03A4},	// #awbb_GamutHeightThr1

	// SceneDetection Thresholds
	{0x002A, 0x0D92},
	{0x0F12, 0x0CE4},	//0A8C	// #awbb_SunnyBr
	{0x0F12, 0x0096},	// #awbb_Sunny_NBzone
	{0x002A, 0x0E86},
	{0x0F12, 0x0216},	// #awbb_OutdoorWP_r
	{0x0F12, 0x029F},	// #awbb_OutdoorWP_b
	{0x002A, 0x0D96},
	{0x0F12, 0x0580},	// #awbb_CloudyBr
	{0x0F12, 0x0096},	// #awbb_Cloudy_NBzone
	{0x002A, 0x0DB2},
	{0x0F12, 0x00DA},	// #awbb_CloudyRB
	{0x002A, 0x0D9A},
	{0x0F12, 0x000A},	// #awbb_Cloudy_BdivRzone
	{0x002A, 0x0DB4},
	{0x0F12, 0x0459},	// #awbb_LowTempRB
	{0x002A, 0x0DA4},
	{0x0F12, 0x000E},	// #awbb_LowTemp_RBzone
	{0x002A, 0x0D64},
	{0x0F12, 0x0040},	// #awbb_DarkBr
	{0x002A, 0x0DA6},
	{0x0F12, 0x001E},	// #awbb_LowBr_NBzone
	{0x002A, 0x0D9C},
	{0x0F12, 0x001B},	// #awbb_MacbethGamut_WidthZone
	{0x0F12, 0x000E},	// #awbb_MacbethGamut_HeightZone
	{0x0F12, 0x0008},	// #awbb_MacbethGamut_WidthZone2
	{0x0F12, 0x0004},	// #awbb_MacbethGamut_HeightZone2

	// AWB Debug.(Outdoor Pink)
	{0x002A, 0x0E30},
	{0x0F12, 0x0000},	//	#awbb_OutdoorFltrSz (outdoor WB를 moving average filtering)

	// UseInvalidOutdoor option
	{0x002A, 0x0D88},
	{0x0F12, 0x0001},	// #awbb_Use_InvalidOutDoor

	// AWB input Y-Filter setting
	{0x002A, 0x0C48},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x002A, 0x0CC4},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high

	{0x002A, 0x0DC2},
	{0x0F12, 0x0030},	// #awbb_GnCurPntImmunity
	{0x0F12, 0x00C8},	// #awbb_GnFarPntImmunity
	{0x0F12, 0x012C},	// #awbb_GnCurPntLongJump
	{0x0F12, 0x0258},	// #awbb_GainsMaxMove
	{0x0F12, 0x0003},	// #awbb_GnMinMatchToJump

	// RB Gain Offset
	//WRITE 70000E32	FFE0	//#awbb_RGainOff
	//WRITE 70000E34	FFe0	//#awbb_BGainOff


	//=================================================================================================
	//	Set CCM
	//=================================================================================================
	// CCM Start Address
	{0x002A, 0x06D0},
	{0x0F12, 0x2800},   //#TVAR_wbt_pBaseCcmsAddr[0]
	{0x0F12, 0x7000},
	{0x0F12, 0x2824},   //#TVAR_wbt_pBaseCcmsAddr[1]
	{0x0F12, 0x7000},
	{0x0F12, 0x2848},   //#TVAR_wbt_pBaseCcmsAddr[2]
	{0x0F12, 0x7000},
	{0x0F12, 0x286C},   //#TVAR_wbt_pBaseCcmsAddr[3]
	{0x0F12, 0x7000},
	{0x0F12, 0x2890},   //#TVAR_wbt_pBaseCcmsAddr[4]
	{0x0F12, 0x7000},
	{0x0F12, 0x28B4},   //#TVAR_wbt_pBaseCcmsAddr[5]
	{0x0F12, 0x7000},
	{0x002A, 0x06EC},
	{0x0F12, 0x28D8},   //#TVAR_wbt_pOutdoorCcm
	{0x0F12, 0x7000},

	{0x002A, 0x2800},
	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	// Outdoor CCM
	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEA4},
	{0x0F12, 0x020F},
	{0x0F12, 0xFF8F},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},



	//=================================================================================================
	//	Set NB
	//=================================================================================================
	{0x002A, 0x07EA},
	{0x0F12, 0x0000},	//afit_bUseNoiseInd 0 : NB 1: Noise Index

	// param_start	SARR_uNormBrInDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrInDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrInDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrInDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrInDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrInDoor[4]

	// param_start	SARR_uNormBrOutDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrOutDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrOutDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrOutDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrOutDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrOutDoor[4]



	//=================================================================================================
	//	Set AFIT
	//=================================================================================================
	// AFIT Start Address
	{0x002A, 0x0814},
	{0x0F12, 0x082C},	//#TVAR_afit_pBaseVals
	{0x0F12, 0x7000},	//#TVAR_afit_pBaseVals

	// AFIT Table	//0223
	{0x002A, 0x082C},
	{0x0F12, 0x0010},	//BRIGHTNESS
	{0x0F12, 0xFFF0},	//CONTRAST
	{0x0F12, 0xFFD0},	//SATURATION
	{0x0F12, 0x0000},	//SHARP_BLUR
	{0x0F12, 0x0000},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x003B},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                   bin: desparity low
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"                bin: desparity high
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x06FF},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x030A},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"               ]negati fine
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"                  ]low fine
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"                ]high fine
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"                   ]high low thres
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0005},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01AA},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0006},	//BRIGHTNESS
	{0x0F12, 0x0000},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0000},	//SHARP_BLUR
	{0x0F12, 0x0000},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0021},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                  bin: desparity low
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x08B7},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0308},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01A0},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0021},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0089},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0202},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0202},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0206},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x803C},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0019},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0205},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0000},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8050},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0019},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0000},	//UVDenoise_iYLowThresh
	{0x0F12, 0x0000},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0105},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0200},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x805A},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[0]
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[1]
	{0x0F12, 0x0800},	// # afit_pConstBaseVals[2]
	{0x0F12, 0x0600},	// # afit_pConstBaseVals[3]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[4]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[5]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[6]
	{0x0F12, 0x0300},	// # afit_pConstBaseVals[7]
	{0x0F12, 0x0002},	// # afit_pConstBaseVals[8]
	{0x0F12, 0x0400},	// # afit_pConstBaseVals[9]
	{0x0F12, 0x0106},	// # afit_pConstBaseVals[10]
	{0x0F12, 0x0005},	// # afit_pConstBaseVals[11]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[12]
	{0x0F12, 0x0003},	// # afit_pConstBaseVals[13]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[14]
	{0x0F12, 0xFFD6},	// # afit_pConstBaseVals[15]
	{0x0F12, 0x53C1},	// # afit_pConstBaseVals[16]
	{0x0F12, 0xE1FE},	// # afit_pConstBaseVals[17]
	{0x0F12, 0x0001},	// # afit_pConstBaseVals[18]

	{0x002A, 0x01EA},
	{0x0F12, 0x0000},	//#REG_TC_UserSharpBlur
	{0x002A, 0x0E32},
	{0x0F12, 0xFFF0},	//#awbb_RGainOff
	{0x0F12, 0x0000},	//#awbb_BGainOff

	// For fast AE
	{0x002A, 0x0FAC},
	{0x0F12, 0x0001},	// AE Gain output [0]    default : 0x10
	{0x0F12, 0x0008},	// AE Gain output [1]    default : 0x20
	{0x0F12, 0x0015},	// AE Gain output [2]    default : 0x40

	// For fast AWB
	{0x002A, 0x0DCA},
	{0x0F12, 0x0001},	// awbb_GnMinMatchToJump = 0x02 (default:0x03)
	{0x0028, 0x7000},
	{0x002A, 0x0E98},
	{0x0F12, 0x0300},	//bp_uMaxBrightnessFactor 0180 -> 0300
	{0x002A, 0x0E9E},
	{0x0F12, 0x0300},	//bp_uMinBrightnessFactor 0180 -> 0300

	// Update Changed Registers
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},

	{0x0028, 0xD000},
	{0x002A, 0x1088},
	{0x0F12, 0x05D5},

	{0x0028, 0xD000},
	{0x002A, 0x1094},
	{0x0F12, 0x0010},

	{0x0028, 0xD000},
	{0x002A, 0x1082},
	{0x0F12, 0x03FF},

	{0x0028, 0xD000},
	{0x002A, 0x1084},
	{0x0F12, 0x03FF},

    {SEQUENCE_WAIT_MS, 0x0064},
    {SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_init_320x240[] =
{
	{0x0028, 0xD000},
	{0x002A, 0x1030},
	{0x0F12, 0x0000},
	{0x002A, 0x0014},
	{0x0F12, 0x0001},

	{SEQUENCE_WAIT_MS , 0x0014},// Delay 100 -> 20

	{0x0028, 0x7000},
	{0x002A, 0x2080},
	{0x0F12, 0xB510},	// 700020800
	{0x0F12, 0xF000},	// 700020822
	{0x0F12, 0xF8FC},	// 700020844
	{0x0F12, 0xBC10},	// 700020866
	{0x0F12, 0xBC08},	// 700020888
	{0x0F12, 0x4718},	// 7000208AA
	{0x0F12, 0xB5FE},	// 7000208CC
	{0x0F12, 0x0007},	// 7000208EE
	{0x0F12, 0x683C},	// 700020900
	{0x0F12, 0x687E},	// 700020922
	{0x0F12, 0x1DA5},	// 700020944
	{0x0F12, 0x88A0},	// 700020966
	{0x0F12, 0x2800},	// 700020988
	{0x0F12, 0xD00B},	// 7000209AA
	{0x0F12, 0x88A8},	// 7000209CC
	{0x0F12, 0x2800},	// 7000209EE
	{0x0F12, 0xD008},	// 700020A00
	{0x0F12, 0x8820},	// 700020A22
	{0x0F12, 0x8829},	// 700020A44
	{0x0F12, 0x4288},	// 700020A66
	{0x0F12, 0xD301},	// 700020A88
	{0x0F12, 0x1A40},	// 700020AAA
	{0x0F12, 0xE000},	// 700020ACC
	{0x0F12, 0x1A08},	// 700020AEE
	{0x0F12, 0x9001},	// 700020B00
	{0x0F12, 0xE001},	// 700020B22
	{0x0F12, 0x2019},	// 700020B44
	{0x0F12, 0x9001},	// 700020B66
	{0x0F12, 0x497B},	// 700020B88
	{0x0F12, 0x466B},	// 700020BAA
	{0x0F12, 0x8A48},	// 700020BCC
	{0x0F12, 0x8118},	// 700020BEE
	{0x0F12, 0x8A88},	// 700020C00
	{0x0F12, 0x8158},	// 700020C22
	{0x0F12, 0x4879},	// 700020C44
	{0x0F12, 0x8940},	// 700020C66
	{0x0F12, 0x0040},	// 700020C88
	{0x0F12, 0x2103},	// 700020CAA
	{0x0F12, 0xF000},	// 700020CCC
	{0x0F12, 0xF902},	// 700020CEE
	{0x0F12, 0x88A1},	// 700020D00
	{0x0F12, 0x4288},	// 700020D22
	{0x0F12, 0xD908},	// 700020D44
	{0x0F12, 0x8828},	// 700020D66
	{0x0F12, 0x8030},	// 700020D88
	{0x0F12, 0x8868},	// 700020DAA
	{0x0F12, 0x8070},	// 700020DCC
	{0x0F12, 0x88A8},	// 700020DEE
	{0x0F12, 0x6038},	// 700020E00
	{0x0F12, 0xBCFE},	// 700020E22
	{0x0F12, 0xBC08},	// 700020E44
	{0x0F12, 0x4718},	// 700020E66
	{0x0F12, 0x88A9},	// 700020E88
	{0x0F12, 0x4288},	// 700020EAA
	{0x0F12, 0xD906},	// 700020ECC
	{0x0F12, 0x8820},	// 700020EEE
	{0x0F12, 0x8030},	// 700020F00
	{0x0F12, 0x8860},	// 700020F22
	{0x0F12, 0x8070},	// 700020F44
	{0x0F12, 0x88A0},	// 700020F66
	{0x0F12, 0x6038},	// 700020F88
	{0x0F12, 0xE7F2},	// 700020FAA
	{0x0F12, 0x9801},	// 700020FCC
	{0x0F12, 0xA902},	// 700020FEE
	{0x0F12, 0xF000},	// 700021000
	{0x0F12, 0xF8EE},	// 700021022
	{0x0F12, 0x0033},	// 700021044
	{0x0F12, 0x0029},	// 700021066
	{0x0F12, 0x9A02},	// 700021088
	{0x0F12, 0x0020},	// 7000210AA
	{0x0F12, 0xF000},	// 7000210CC
	{0x0F12, 0xF8F0},	// 7000210EE
	{0x0F12, 0x6038},	// 700021100
	{0x0F12, 0xE7E6},	// 700021122
	{0x0F12, 0xB5F0},	// 700021144
	{0x0F12, 0xB08B},	// 700021166
	{0x0F12, 0x0006},	// 700021188
	{0x0F12, 0x2000},	// 7000211AA
	{0x0F12, 0x9004},	// 7000211CC
	{0x0F12, 0x6835},	// 7000211EE
	{0x0F12, 0x6874},	// 700021200
	{0x0F12, 0x68B0},	// 700021222
	{0x0F12, 0x900A},	// 700021244
	{0x0F12, 0x68F0},	// 700021266
	{0x0F12, 0x9009},	// 700021288
	{0x0F12, 0x4F60},	// 7000212AA
	{0x0F12, 0x8979},	// 7000212CC
	{0x0F12, 0x084A},	// 7000212EE
	{0x0F12, 0x88A8},	// 700021300
	{0x0F12, 0x88A3},	// 700021322
	{0x0F12, 0x4298},	// 700021344
	{0x0F12, 0xD300},	// 700021366
	{0x0F12, 0x0018},	// 700021388
	{0x0F12, 0xF000},	// 7000213AA
	{0x0F12, 0xF8E1},	// 7000213CC
	{0x0F12, 0x9007},	// 7000213EE
	{0x0F12, 0x0021},	// 700021400
	{0x0F12, 0x0028},	// 700021422
	{0x0F12, 0xAA04},	// 700021444
	{0x0F12, 0xF000},	// 700021466
	{0x0F12, 0xF8E3},	// 700021488
	{0x0F12, 0x9006},	// 7000214AA
	{0x0F12, 0x88A8},	// 7000214CC
	{0x0F12, 0x2800},	// 7000214EE
	{0x0F12, 0xD102},	// 700021500
	{0x0F12, 0x27FF},	// 700021522
	{0x0F12, 0x1C7F},	// 700021544
	{0x0F12, 0xE047},	// 700021566
	{0x0F12, 0x88A0},	// 700021588
	{0x0F12, 0x2800},	// 7000215AA
	{0x0F12, 0xD101},	// 7000215CC
	{0x0F12, 0x2700},	// 7000215EE
	{0x0F12, 0xE042},	// 700021600
	{0x0F12, 0x8820},	// 700021622
	{0x0F12, 0x466B},	// 700021644
	{0x0F12, 0x8198},	// 700021666
	{0x0F12, 0x8860},	// 700021688
	{0x0F12, 0x81D8},	// 7000216AA
	{0x0F12, 0x8828},	// 7000216CC
	{0x0F12, 0x8118},	// 7000216EE
	{0x0F12, 0x8868},	// 700021700
	{0x0F12, 0x8158},	// 700021722
	{0x0F12, 0xA802},	// 700021744
	{0x0F12, 0xC803},	// 700021766
	{0x0F12, 0xF000},	// 700021788
	{0x0F12, 0xF8D2},	// 7000217AA
	{0x0F12, 0x9008},	// 7000217CC
	{0x0F12, 0x8ABA},	// 7000217EE
	{0x0F12, 0x9808},	// 700021800
	{0x0F12, 0x466B},	// 700021822
	{0x0F12, 0x4342},	// 700021844
	{0x0F12, 0x9202},	// 700021866
	{0x0F12, 0x8820},	// 700021888
	{0x0F12, 0x8198},	// 7000218AA
	{0x0F12, 0x8860},	// 7000218CC
	{0x0F12, 0x81D8},	// 7000218EE
	{0x0F12, 0x980A},	// 700021900
	{0x0F12, 0x9903},	// 700021922
	{0x0F12, 0xF000},	// 700021944
	{0x0F12, 0xF8C4},	// 700021966
	{0x0F12, 0x9A02},	// 700021988
	{0x0F12, 0x17D1},	// 7000219AA
	{0x0F12, 0x0E09},	// 7000219CC
	{0x0F12, 0x1889},	// 7000219EE
	{0x0F12, 0x1209},	// 700021A00
	{0x0F12, 0x4288},	// 700021A22
	{0x0F12, 0xDD1F},	// 700021A44
	{0x0F12, 0x8820},	// 700021A66
	{0x0F12, 0x466B},	// 700021A88
	{0x0F12, 0x8198},	// 700021AAA
	{0x0F12, 0x8860},	// 700021ACC
	{0x0F12, 0x81D8},	// 700021AEE
	{0x0F12, 0x980A},	// 700021B00
	{0x0F12, 0x9903},	// 700021B22
	{0x0F12, 0xF000},	// 700021B44
	{0x0F12, 0xF8B4},	// 700021B66
	{0x0F12, 0x9001},	// 700021B88
	{0x0F12, 0x8828},	// 700021BAA
	{0x0F12, 0x466B},	// 700021BCC
	{0x0F12, 0x8118},	// 700021BEE
	{0x0F12, 0x8868},	// 700021C00
	{0x0F12, 0x8158},	// 700021C22
	{0x0F12, 0x980A},	// 700021C44
	{0x0F12, 0x9902},	// 700021C66
	{0x0F12, 0xF000},	// 700021C88
	{0x0F12, 0xF8AA},	// 700021CAA
	{0x0F12, 0x8AB9},	// 700021CCC
	{0x0F12, 0x9A08},	// 700021CEE
	{0x0F12, 0x4351},	// 700021D00
	{0x0F12, 0x17CA},	// 700021D22
	{0x0F12, 0x0E12},	// 700021D44
	{0x0F12, 0x1851},	// 700021D66
	{0x0F12, 0x120A},	// 700021D88
	{0x0F12, 0x9901},	// 700021DAA
	{0x0F12, 0xF000},	// 700021DCC
	{0x0F12, 0xF890},	// 700021DEE
	{0x0F12, 0x0407},	// 700021E00
	{0x0F12, 0x0C3F},	// 700021E22
	{0x0F12, 0xE000},	// 700021E44
	{0x0F12, 0x2700},	// 700021E66
	{0x0F12, 0x8820},	// 700021E88
	{0x0F12, 0x466B},	// 700021EAA
	{0x0F12, 0xAA05},	// 700021ECC
	{0x0F12, 0x8198},	// 700021EEE
	{0x0F12, 0x8860},	// 700021F00
	{0x0F12, 0x81D8},	// 700021F22
	{0x0F12, 0x8828},	// 700021F44
	{0x0F12, 0x8118},	// 700021F66
	{0x0F12, 0x8868},	// 700021F88
	{0x0F12, 0x8158},	// 700021FAA
	{0x0F12, 0xA802},	// 700021FCC
	{0x0F12, 0xC803},	// 700021FEE
	{0x0F12, 0x003B},	// 700022000
	{0x0F12, 0xF000},	// 700022022
	{0x0F12, 0xF895},	// 700022044
	{0x0F12, 0x88A1},	// 700022066
	{0x0F12, 0x88A8},	// 700022088
	{0x0F12, 0x003A},	// 7000220AA
	{0x0F12, 0xF000},	// 7000220CC
	{0x0F12, 0xF898},	// 7000220EE
	{0x0F12, 0x0004},	// 700022100
	{0x0F12, 0xA804},	// 700022122
	{0x0F12, 0xC803},	// 700022144
	{0x0F12, 0x9A09},	// 700022166
	{0x0F12, 0x9B07},	// 700022188
	{0x0F12, 0xF000},	// 7000221AA
	{0x0F12, 0xF889},	// 7000221CC
	{0x0F12, 0xA806},	// 7000221EE
	{0x0F12, 0xC805},	// 700022200
	{0x0F12, 0x0021},	// 700022222
	{0x0F12, 0xF000},	// 700022244
	{0x0F12, 0xF88C},	// 700022266
	{0x0F12, 0x6030},	// 700022288
	{0x0F12, 0xB00B},	// 7000222AA
	{0x0F12, 0xBCF0},	// 7000222CC
	{0x0F12, 0xBC08},	// 7000222EE
	{0x0F12, 0x4718},	// 700022300
	{0x0F12, 0xB510},	// 700022322
	{0x0F12, 0x6800},	// 700022344
	{0x0F12, 0x2805},	// 700022366
	{0x0F12, 0xD11E},	// 700022388
	{0x0F12, 0x481D},	// 7000223AA
	{0x0F12, 0x491D},	// 7000223CC
	{0x0F12, 0x8800},	// 7000223EE
	{0x0F12, 0x8348},	// 700022400
	{0x0F12, 0x2301},	// 700022422
	{0x0F12, 0x31A0},	// 700022444
	{0x0F12, 0x720B},	// 700022466
	{0x0F12, 0x2000},	// 700022488
	{0x0F12, 0x000A},	// 7000224AA
	{0x0F12, 0x3A60},	// 7000224CC
	{0x0F12, 0x7150},	// 7000224EE
	{0x0F12, 0x71CB},	// 700022500
	{0x0F12, 0x22FF},	// 700022522
	{0x0F12, 0x3980},	// 700022544
	{0x0F12, 0x75CA},	// 700022566
	{0x0F12, 0x764A},	// 700022588
	{0x0F12, 0x000A},	// 7000225AA
	{0x0F12, 0x3260},	// 7000225CC
	{0x0F12, 0x7210},	// 7000225EE
	{0x0F12, 0x7250},	// 700022600
	{0x0F12, 0x2008},	// 700022622
	{0x0F12, 0x7488},	// 700022644
	{0x0F12, 0x74C8},	// 700022666
	{0x0F12, 0x4813},	// 700022688
	{0x0F12, 0x217E},	// 7000226AA
	{0x0F12, 0x43C9},	// 7000226CC
	{0x0F12, 0x8101},	// 7000226EE
	{0x0F12, 0x2306},	// 700022700
	{0x0F12, 0x5EC0},	// 700022722
	{0x0F12, 0xF000},	// 700022744
	{0x0F12, 0xF86C},	// 700022766
	{0x0F12, 0xBC10},	// 700022788
	{0x0F12, 0xBC08},	// 7000227AA
	{0x0F12, 0x4718},	// 7000227CC
	{0x0F12, 0xB570},	// 7000227EE
	{0x0F12, 0x2200},	// 700022800
	{0x0F12, 0x490E},	// 700022822
	{0x0F12, 0x480E},	// 700022844
	{0x0F12, 0x2401},	// 700022866
	{0x0F12, 0xF000},	// 700022888
	{0x0F12, 0xF86A},	// 7000228AA
	{0x0F12, 0x0022},	// 7000228CC
	{0x0F12, 0x490D},	// 7000228EE
	{0x0F12, 0x480D},	// 700022900
	{0x0F12, 0x2502},	// 700022922
	{0x0F12, 0xF000},	// 700022944
	{0x0F12, 0xF864},	// 700022966
	{0x0F12, 0x490C},	// 700022988
	{0x0F12, 0x480D},	// 7000229AA
	{0x0F12, 0x002A},	// 7000229CC
	{0x0F12, 0xF000},	// 7000229EE
	{0x0F12, 0xF85F},	// 700022A00
	{0x0F12, 0xBC70},	// 700022A22
	{0x0F12, 0xBC08},	// 700022A44
	{0x0F12, 0x4718},	// 700022A66
	{0x0F12, 0x1A28},	// 700022A88
	{0x0F12, 0x7000},	// 700022AAA
	{0x0F12, 0x0D64},	// 700022ACC
	{0x0F12, 0x7000},	// 700022AEE
	{0x0F12, 0x2370},	// 700022B00
	{0x0F12, 0x7000},	// 700022B22
	{0x0F12, 0x1C14},	// 700022B44
	{0x0F12, 0x7000},	// 700022B66
	{0x0F12, 0x14FA},	// 700022B88
	{0x0F12, 0x7000},	// 700022BAA
	{0x0F12, 0x2115},	// 700022BCC
	{0x0F12, 0x7000},	// 700022BEE
	{0x0F12, 0x622F},	// 700022C00
	{0x0F12, 0x0000},	// 700022C22
	{0x0F12, 0x208D},	// 700022C44
	{0x0F12, 0x7000},	// 700022C66
	{0x0F12, 0x6445},	// 700022C88
	{0x0F12, 0x0000},	// 700022CAA
	{0x0F12, 0x2233},	// 700022CCC
	{0x0F12, 0x7000},	// 700022CEE
	{0x0F12, 0x3B5B},	// 700022D00
	{0x0F12, 0x0000},	// 700022D22
	{0x0F12, 0x4778},	// 700022D44
	{0x0F12, 0x46C0},	// 700022D66
	{0x0F12, 0xF004},	// 700022D88
	{0x0F12, 0xE51F},	// 700022DAA
	{0x0F12, 0xA464},	// 700022DCC
	{0x0F12, 0x0000},	// 700022DEE
	{0x0F12, 0x4778},	// 700022E00
	{0x0F12, 0x46C0},	// 700022E22
	{0x0F12, 0xC000},	// 700022E44
	{0x0F12, 0xE59F},	// 700022E66
	{0x0F12, 0xFF1C},	// 700022E88
	{0x0F12, 0xE12F},	// 700022EAA
	{0x0F12, 0x6009},	// 700022ECC
	{0x0F12, 0x0000},	// 700022EEE
	{0x0F12, 0x4778},	// 700022F00
	{0x0F12, 0x46C0},	// 700022F22
	{0x0F12, 0xC000},	// 700022F44
	{0x0F12, 0xE59F},	// 700022F66
	{0x0F12, 0xFF1C},	// 700022F88
	{0x0F12, 0xE12F},	// 700022FAA
	{0x0F12, 0x622F},	// 700022FCC
	{0x0F12, 0x0000},	// 700022FEE
	{0x0F12, 0x4778},	// 700023000
	{0x0F12, 0x46C0},	// 700023022
	{0x0F12, 0xC000},	// 700023044
	{0x0F12, 0xE59F},	// 700023066
	{0x0F12, 0xFF1C},	// 700023088
	{0x0F12, 0xE12F},	// 7000230AA
	{0x0F12, 0x5F49},	// 7000230CC
	{0x0F12, 0x0000},	// 7000230EE
	{0x0F12, 0x4778},	// 700023100
	{0x0F12, 0x46C0},	// 700023122
	{0x0F12, 0xC000},	// 700023144
	{0x0F12, 0xE59F},	// 700023166
	{0x0F12, 0xFF1C},	// 700023188
	{0x0F12, 0xE12F},	// 7000231AA
	{0x0F12, 0x5FC7},	// 7000231CC
	{0x0F12, 0x0000},	// 7000231EE
	{0x0F12, 0x4778},	// 700023200
	{0x0F12, 0x46C0},	// 700023222
	{0x0F12, 0xC000},	// 700023244
	{0x0F12, 0xE59F},	// 700023266
	{0x0F12, 0xFF1C},	// 700023288
	{0x0F12, 0xE12F},	// 7000232AA
	{0x0F12, 0x5457},	// 7000232CC
	{0x0F12, 0x0000},	// 7000232EE
	{0x0F12, 0x4778},	// 700023300
	{0x0F12, 0x46C0},	// 700023322
	{0x0F12, 0xC000},	// 700023344
	{0x0F12, 0xE59F},	// 700023366
	{0x0F12, 0xFF1C},	// 700023388
	{0x0F12, 0xE12F},	// 7000233AA
	{0x0F12, 0x5FA3},	// 7000233CC
	{0x0F12, 0x0000},	// 7000233EE
	{0x0F12, 0x4778},	// 700023400
	{0x0F12, 0x46C0},	// 700023422
	{0x0F12, 0xC000},	// 700023444
	{0x0F12, 0xE59F},	// 700023466
	{0x0F12, 0xFF1C},	// 700023488
	{0x0F12, 0xE12F},	// 7000234AA
	{0x0F12, 0x51F9},	// 7000234CC
	{0x0F12, 0x0000},	// 7000234EE
	{0x0F12, 0x4778},	// 700023500
	{0x0F12, 0x46C0},	// 700023522
	{0x0F12, 0xC000},	// 700023544
	{0x0F12, 0xE59F},	// 700023566
	{0x0F12, 0xFF1C},	// 700023588
	{0x0F12, 0xE12F},	// 7000235AA
	{0x0F12, 0x7633},	// 7000235CC
	{0x0F12, 0x0000},	// 7000235EE
	{0x0F12, 0x4778},	// 700023600
	{0x0F12, 0x46C0},	// 700023622
	{0x0F12, 0xC000},	// 700023644
	{0x0F12, 0xE59F},	// 700023666
	{0x0F12, 0xFF1C},	// 700023688
	{0x0F12, 0xE12F},	// 7000236AA
	{0x0F12, 0xA007},	// 7000236CC
	{0x0F12, 0x0000},	// 7000236EE
	{0x0F12, 0x0360},	// 700023700
	{0x0F12, 0x6F4E},	// 700023722
	{0x0F12, 0x2076},	// 700023744
	{0x0F12, 0x3831},	// 700023766
	{0x0F12, 0x3220},	// 700023788
	{0x0F12, 0x3030},	// 7000237AA
	{0x0F12, 0x0038},	// 7000237CC
	{0x0F12, 0xE010},	// 7000237EE
	{0x0F12, 0x0208},	// 700023800
	{0x0F12, 0x0038},	// 700023822
	// End of Trap and Patch (Last : 70002382h) // Total Size 772 (0x0304)
	{0x0028, 0xD000},
	{0x002A, 0x1000},
	{0x0F12, 0x0001},

	{0x0028, 0x7000},
	{0x002A, 0x0ADC},
	{0x0F12, 0x0AF0},	//#setot_uOnlineClocksDiv4OnlineC0             //2800
	{0x002A, 0x0AE2},
	{0x0F12, 0x222E},	//#setot_usSetRomWaitStatesSetRomThreshold4KHz //ROM Wait Threshold to 35MHz
	{0x002A, 0x0B94},
	{0x0F12, 0x0500},	//#awbb_GainsInit_0_:R    insInit
	{0x0F12, 0x0400},	//#awbb_GainsInit_1_:G    insInit
	{0x0F12, 0x0700},	//#awbb_GainsInit_2_:B    insInit
	{0x002A, 0x04A0},
	{0x0F12, 0x8000},	//#lt_uLeiInit:AE start   Init:AE
	{0x002A, 0x049A},
	{0x0F12, 0x003B},	//#lt_uMinExp             Exp



	//=================================================================================================
	//	Set CIS/APS/Analog
	//=================================================================================================
	// This registers are for FACTORY ONLY. If you change it without prior notification,
	// YOU are RESPONSIBLE for the FAILURE that will happen in the future.
	//=================================================================================================
	{0x0028, 0xD000},
	{0x002A, 0xF106},
	{0x0F12, 0x0001},	// L-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xF206},
	{0x0F12, 0x0001},	// F-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xC202},
	{0x0F12, 0x0700},	// tgr_coarse_integration_time[15:0]: revised by Ana 080115
	{0x002A, 0xF414},
	{0x0F12, 0x0030},	// aig_adc_sat[7:0] (aig_adc_sat[7:4]+1)*33mV + 690mV
	{0x002A, 0xC204},
	{0x0F12, 0x0100},	// tgr_analogue_code_global[12:0] Analog gain X8
	{0x002A, 0xF402},
	{0x0F12, 0x0092},	// aig_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_offset2[7:0]: revised by Ana 080425
	{0x002A, 0xF700},
	{0x0F12, 0x0040},	// bpradlc_control[7:0]: revised by Ana 080529 // bpradlc_nactive_pedestal[7:5],bpradlc_passthrough[1],bpradlc_bypass[0]
	{0x002A, 0xF708},
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_r[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gr[7:0]: revised by Tg 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gb[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_b[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_total[7:0]: revised by TG 080529
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_r[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gr[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gb[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_b[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_total[7:0]: revised by Ana 080425
	{0x0F12, 0x0001},	// bpradlc_adlc_en[7:0]: revised by TG 080529	// bpradlc_f_l_adlc_en[2],bpradlc_f_adlc_en[1],bpradlc_h_adlc_en[0]
	{0x0F12, 0x0015},	// bpradlc_adlc_option[7:0]: revised by TG 080425 // bpradlc_h_adlc_ch_sel[4], // bpradlc_max_data_clip_sel[3:2],bpradlc_f_adlc_max_data_clip_sel[1:0]
	{0x0F12, 0x0001},	// bpradlc_adlc_bpr_en[7:0] adlc_bpr_enable: revised by Ana 080112
	{0x0F12, 0x0040},	// bpradlc_adlc_bpr_thresh[7:0]: 080425
	{0x002A, 0xF48A},
	{0x0F12, 0x0048},	// aig_ld_ptr7[9:0] line OB start addr(04h~48h): revised by Ana 080911
	{0x002A, 0xF10A},
	{0x0F12, 0x008B},	// tgr_h_desc0_addr_end[10:0] line OB end addr(47h~8Bh): revised by Ana 080911 // line OB start - line OB end = 67d
	{0x002A, 0xF900},
	{0x0F12, 0x0067},	// cfpn_config: revised by Ana 080425 // cfpn_ref_gain[9:7],cpfn_ref_lines[6:5],cfpn_hbinning[4],cfpn_output_direct[3],// cfpn_overflow_protect[2],cpfn_passthrough[1],cfpn_bypass[0]
	{0x002A, 0xF406},
	{0x0F12, 0x0092},	// aig_cfpn_ref_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_cfpn_ref_offset2[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_cfpn_ref_gain[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_ld_ctrl[1:0] aig_ldb_en[1], aig_ld_en[0]
	{0x0F12, 0x0000},	// aig_row_id_ctrl[1:0]
	{0x002A, 0xF442},
	{0x0F12, 0x0000},	// aig_vavg[0]
	{0x0F12, 0x0000},	// aig_havg[0]=1b @H-avg mode: revised by Ana 080116
	{0x002A, 0xF448},
	{0x0F12, 0x0000},	// aig_sl_off[0]
	{0x002A, 0xF454},
	{0x0F12, 0x0001},	//Power save enable
	{0x0F12, 0x0001},	// aig_blst_en[0]
	{0x0F12, 0x0010},	// aig_blst_en_cintr[15:0]
	{0x0F12, 0x0000},	// aig_dshut_en[0]
	{0x002A, 0xF41A},
	{0x0F12, 0x00FF},	// aig_comp_bias[7:0] aig_comp2_bias[7:4], aig_comp1_bias[3:0]: revised by Ana 081013
	{0x0F12, 0x0003},	// aig_pix_bias[3:0]
	{0x002A, 0xF420},
	{0x0F12, 0x0030},	// aig_clp_lvl[7:0]: revised by Ana 080910(refer to 6AA)
	{0x002A, 0xF410},
	{0x0F12, 0x0001},	// aig_clp_sl_ctrl[0]
	{0x0F12, 0x0000},	// aig_cds_test[7:0], aig_cds_test[1]=1b @H-avg mode current save: revised by Ana 080116
	{0x002A, 0xF416},
	{0x0F12, 0x0001},	// aig_rmp_option[7:0]
	{0x002A, 0xF424},
	{0x0F12, 0x0000},	// aig_ref_option[7:0], aig_ref_option[0]=1b @H-avg mode odd cds off: revised by Ana 080116
	{0x002A, 0xF422},
	{0x0F12, 0x0000},	// aig_monit[7:0]
	{0x002A, 0xF41E},
	{0x0F12, 0x0000},	// aig_pd_pix[0]
	{0x002A, 0xF428},
	{0x0F12, 0x0000},	// aig_pd_cp[1:0] aig_pd_ncp[1], aig_pd_cp[0]
	{0x0F12, 0x0000},	// aig_pd_reg_pix[0]
	{0x0F12, 0x0000},	// aig_pd_reg_rg[0]
	{0x002A, 0xF430},
	{0x0F12, 0x0000},	// aig_pd_reg_tgsl[0]
	{0x0F12, 0x0000},	// aig_pd_reg_ntg[0]
	{0x0F12, 0x0008},	// aig_rosc_tune_cp[3:0]: revised by Ana 080418
	{0x0F12, 0x0005},	// aig_rosc_tune_ncp[3:0]
	{0x0F12, 0x000F},	// aig_cp_capa[3:0]
	{0x0F12, 0x0001},	// aig_reg_tune_pix[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_rg[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_tgsl[7:0]
	{0x0F12, 0x0010},	// aig_reg_tune_ntg[7:0] 0040 -> 0010
	{0x002A, 0xF4D6},
	{0x0F12, 0x00C0},	// aig_v15_tune[7:0]: revised by Ana 081010 // aig_v15_tune[7:4]=7h @sensor only Mode // aig_v15_tune[7:4]=9h @ISP 7.5fps Mode // aig_v15_tune[7:4]=Dh @ISP 15fps Mode
	{0x0F12, 0x0000},	//aig_vreg_sel[7:0] [9]h_test, [1]reg_sw_stby, [0]aig_reg_sel
	{0x002A, 0xF47C},
	{0x0F12, 0x000C},	//aig_ld_ptr0[4:0]
	{0x0F12, 0x0000},	//aig_ld_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF49A},
	{0x0F12, 0x0008},	//aig_sla_ptr0[3:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_sla_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4A2},
	{0x0F12, 0x0008},	//aig_slb_ptr0[7:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_slb_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4B2},
	{0x0F12, 0x0013},	//aig_rxa_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxa_ptr1[9:0]: revised by Ana 081010
	{0x0F12, 0x0013},	//aig_rxb_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxb_ptr1[9:0]: revised by Ana 081010
	{0x002A, 0xF4AA},
	{0x0F12, 0x009B},	//aig_txa_ptr0[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txa_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_txb_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txb_ptr1[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF474},
	{0x0F12, 0x0017},	//aig_s3_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x005F},	//aig_s3_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0017},	//aig_s4_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x008F},	//aig_s4_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF48C},
	{0x0F12, 0x0017},	//aig_clp_sl_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_clp_sl_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF4C8},
	{0x0F12, 0x0163},	//aig_off_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0193},	//aig_rmp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF490},
	{0x0F12, 0x0191},	//aig_comp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF418},
	{0x0F12, 0x0083},	//aig_dbs_option[7:0]: revised by Ana 081010

	//For ESD Check
	{0x0028, 0x7000},
	{0x002A, 0x0132},
	{0x0F12, 0xAAAA},	//#define REG_FWpid (For ESD Check)
	// * note : ESD Check Method.
	//	1. if (7000.0132 != AAAA ) reset // ( S/W reg. check)
	//	2. if (D000.1002 != 0000 ) reset // (internal Exception)

	//Set FPN Gain Input
	{0x002A, 0x1176},
	{0x0F12, 0x0020},	// fpn_GainInput[0]
	{0x0F12, 0x0040},	// fpn_GainInput[1]
	{0x0F12, 0x0080},	// fpn_GainInput[2]
	{0x0F12, 0x0100},	// fpn_GainInput[3]
	{0x0F12, 0x0014},	// fpn_GainOutput[0]
	{0x0F12, 0x000A},	// fpn_GainOutput[1]
	{0x0F12, 0x0008},	// fpn_GainOutput[2]
	{0x0F12, 0x0004},	// fpn_GainOutput[3]

	// Set CFPN Canceller
	{0x002A, 0x116E},
	{0x0F12, 0x0000},	// OvflProtect
	{0x0F12, 0x0000},	// bypassThrough
	{0x0F12, 0x0000},	// bypass
	{0x0F12, 0x0002},	// fpn.CollectMethod	// 0 : Center	1 : Right	2: LEFT
	{0x002A, 0x0AE8},
	{0x0F12, 0x0000},	// setot_bSendFpnToISP = ??

	// Set Forbidden Area
	{0x002A, 0x10EE},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_usRegCount // 0 value --> not use aig table 3
	{0x002A, 0x10F2},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_pSenModesRegsArray3[0]		//real register 0xD000F45A
	{0x002A, 0x1152},
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[48]    //real register 0xD000F4BA
	{0x0F12, 0x0028},	// senHal_SenRegsModes3_pSenModesRegsArray3[49]    //real register 0xD000F4BC
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[50]    //real register 0xD000F4BE
	{0x002A, 0x1148},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[43]    //real register 0xD000F4B0
	{0x002A, 0x1144},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[41]    //real register 0xD000F4AC
	{0x002A, 0x1150},
	{0x0F12, 0x0148},	// senHal_SenRegsModes3_pSenModesRegsArray3[47]    //real register 0xD000F4B8

	// Set AE Target
	{0x002A, 0x0F4C},
	{0x0F12, 0x003D},	// #TVAR_ae_BrAve



	//=================================================================================================
	//	Set Frame Rate
	//=================================================================================================
	{0x002A, 0x0484},
	{0x0F12, 0x61A8},	// #lt_uMaxExp1 	// 50ms
	{0x0F12, 0x0000},
	{0x0F12, 0xE848},	// #lt_uMaxExp2 	// 250ms
	{0x0F12, 0x0001},
	{0x0F12, 0x03C4},	// #lt_uCapMaxExp1	// 133ms
	{0x0F12, 0x0001},
	{0x0F12, 0xE848},	// #lt_uCapMaxExp2	// 250ms
	{0x0F12, 0x0001},
	{0x0F12, 0x04C0},	// #lt_uMaxAnGain1	// x4.75
	{0x0F12, 0x0700},	// #lt_uMaxAnGain2	// x7.0
	{0x0F12, 0x0100},	// #lt_uMaxDigGain
	{0x002A, 0x0F52},
	{0x0F12, 0x0007},	// #ae_StatMode



	//=================================================================================================
	//	Set Flicker  Current Flicker = 7000.164A //Mon_AFC_usFlExpQunat		0001:Off 1045:60Hz	1388:50Hz
	//=================================================================================================

	//1. Auto Flicker 60Hz Start
	{0x002A, 0x0B2E},
	{0x0F12, 0x0001},	// #AFC_Default60Hz 	1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	{0x002A, 0x03F8},
	{0x0F12, 0x007F},	// #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////2. Auto Flicker 50Hz Start
	//WRITE	70000B2E	0000	// #AFC_Default60Hz		1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	//WRITE 700003F8	007F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////3. Manual Flicker 60Hz


	//=================================================================================================
	//	Set PLL
	//=================================================================================================
	// Input CLK
	{0x002A, 0x01B8},
	{0x0F12, 0x5DC0},	//5DC0	//#REG_TC_IPRM_InClockLSBs	//24 -> 24.5M Hz(20081208)
	{0x0F12, 0x0000},	//#REG_TC_IPRM_InClockMSBs

	// Target Sys CLK
	{0x002A, 0x01CC},
	{0x0F12, 0x1770},	// #REG_TC_IPRM_OpClk4KHz_0 			//24Mhz

	// Target PVI CLK
	{0x0F12, 0x2318},	// #REG_TC_IPRM_MinOutRate4KHz_0	//36Mhz
	{0x0F12, 0x2338},	// #REG_TC_IPRM_MaxOutRate4KHz_0

	// PLL ETC
	{0x002A, 0x01C6},
	{0x0F12, 0x0003},	//#REG_TC_IPRM_UseNPviClocks
	{0x0F12, 0x0000},	//#REG_TC_IPRM_UseNMipiClocks
	{0x002A, 0x01C0},
	{0x0F12, 0x0000},	//#EG_TC_IPRM_ValidVActiveLow
	{0x002A, 0x01DE},
	{0x0F12, 0x0001},	//#REG_TC_IPRM_UseRegsAPI
	{0x0F12, 0x0001},	//#REG_TC_IPRM_InitParamsUpdated
	{0x002A, 0x1084},
	{0x0F12, 0x0001},	//#senHal_bUseAnalogBinning //Avg S.S
	{0x002A, 0x10B6},
	{0x0F12, 0x0048},	//#senHal_uMinColsAddAnalogBin



	//=================================================================================================
	//	Set Preview Config
	//=================================================================================================

	//* Preview Cropping Code *//
#ifdef VC_CAMERA_CROP
	{0x002A,	0x020A},
//	{0x0F12,	0x0640},	//#REG_TC_GP_PrevZoomReqInputWidth
//	{0x0F12,	0x0384},	//#REG_TC_GP_PrevZoomReqInputHeight
	{0x0F12,	0x0500},	//#REG_TC_GP_PrevZoomReqInputWidthOfs		0x500(1280)
	{0x0F12,	0x03C0},	//#REG_TC_GP_PrevZoomReqInputHeightOfs		0x3C0(960)

	//* Zoom Update *//
	{0x0F12,	0x0000},	//#REG_TC_GP_PrevZoomReqInputWidthOffset
	{0x0F12,	0x00F0},	//#REG_TC_GP_PrevZoomReqInputHeightOffset

	{0x002A,	0x021A},
	{0x0F12,	0x0001},	// #REG_TC_GP_Inputs_ChangeRequest : 0x021A
#endif
	{0x002A, 0x0242},
	{0x0F12, 0x0140}, //0x0280, //0x0140(320)->640, //090416 800 -> 320, 0x0320,	                 1280 -> 0x500  //0280  // #REG_0TC_PCFG_usWidth						// 640 -> 800 _20081209
	{0x0F12, 0x00f0}, //0x01e0, //0x00f0(240)->480, //090416 600 -> 240, 0x0258, 0x168 -> 360     720 -> 0x2D0	//01e0  // #REG_0TC_PCFG_usHeight						// 480 -> 600 _20081209
	{0x0F12, 0x0005},	// #REG_0TC_PCFG_Format 						// YUV output
	{0x0F12, 0x2338},	// #REG_0TC_PCFG_usMaxOut4KHzRate
	{0x0F12, 0x2318},	// #REG_0TC_PCFG_usMinOut4KHzRate
//	{0x0F12, 0x446C},	// #REG_0TC_PCFG_usMaxOut4KHzRate
//	{0x0F12, 0x444C},	// #REG_0TC_PCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	// #REG_0TC_PCFG_PVIMask   (x2: PCLK rising x0: PCLK Falling)
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_uClockInd
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_usFrTimeType
//	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType		(0 : bin mode, 2 : No-bin mode)
	{0x0F12, 0x02AF},	//03E8},	// #REG_0TC_PCFG_usMaxFrTimeMsecMult10 // 10fps --> 15fps
	{0x0F12, 0x01F4},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 20fps
//	{0x0F12, 0x014D},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 30fps
	{0x002A, 0x0262},
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uPrevMirror -- X/Y flip 20090112
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uCaptureMirror -- X/Y flip 20090112
	//WRITE 70000262 000F  //#REG_0TC_PCFG_uPrevMirror
	//WRITE 70000264 000F  //#REG_0TC_PCFG_uCaptureMirror


	// Apply preview config
	{0x002A, 0x021C},
	{0x0F12, 0x0000},	//#REG_TC_GP_ActivePrevConfig	  //preview config0
	{0x002A, 0x0220},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevOpenAfterChange  //config 변경적용
	{0x002A, 0x01F8},
	{0x0F12, 0x0001},	//#REG_TC_GP_NewConfigSync
	{0x002A, 0x021E},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevConfigChanged



	//=================================================================================================
	//	Set Capture Config
	//=================================================================================================
	//WRITE 70000214	04C0	// #REG_TC_GP_CapZoomReqInputHeight	// Vsize = 1216
	//WRITE 70000216	0000	// #REG_TC_GP_CapZoomReqInputWidthOfs
	//WRITE 70000218	0000	// #REG_TC_GP_CapZoomReqInputHeightOfs
	//WRITE 7000021A	0001	// #REG_TC_GP_InputsChangeRequest
	//WRITE 700001F8	0001	// #REG_TC_GP_NewConfigSync
	//
#ifdef VC_CAMERA_CROP
	//* Capture Cropping Code *//
	{0x0028, 0x7000},
	{0x002A, 0x0212},
	{0x0F12, 0x0500}, //#REG_TC_GP_CapZoomReqInputWidth
	{0x0F12, 0x03C0}, //#REG_TC_GP_CapZoomReqInputHeight
	{0x0F12, 0x0000}, //#REG_TC_GP_CapZoomReqInputWidthOfs
	{0x0F12, 0x00f0}, //#REG_TC_GP_CapZoomReqInputHeightOfs
#endif
	{0x002A, 0x0302},
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uCaptureMode
	{0x0F12, 0x0500},	//#REG_0TC_CCFG_usWidth
	{0x0F12, 0x03C0},	//#REG_0TC_CCFG_usHeight						// Vsize = 1216
	{0x0F12, 0x0005},	//#REG_0TC_CCFG_Format	   //5:YUV,9:JPEG
	{0x0F12, 0x2338},	//#REG_0TC_CCFG_usMaxOut4KHzRate
	{0x0F12, 0x2318},	//#REG_0TC_CCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	//#REG_0TC_CCFG_PVIMask (x2: PCLK rising	x0: PCLK Falling)
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uClockInd
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_usFrTimeType
	{0x0F12, 0x0002},	//#REG_0TC_CCFG_FrRateQualityType     (0 : bin mode, 2 : No-bin mode)
	{0x0F12, 0x09C4},	//07D0},	//#REG_0TC_CCFG_usMaxFrTimeMsecMult10 // 5fps --> 4fps
	{0x0F12, 0x0535},	//#REG_0TC_CCFG_usMinFrTimeMsecMult10 // 7.5fps


	//* Capture Configuration Update *//
	{0x002A, 0x01F8},
	{0x0F12, 0x0001}, //REG_TC_GP_NewConfigSync 0001 : update configuration
	{0x002a, 0x0226},
	{0x0f12, 0x0001}, //REG_TC_GP_CapConfigChanged 0001

	//=================================================================================================
	//	Set Periodic Mismatch (Use default value)
	//=================================================================================================



	//=================================================================================================
	//	Set AE Weights
	//=================================================================================================
	{0x002A, 0x0F5A},
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_0_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_1_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_2_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_3_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_4_
	{0x0F12, 0x0000},	//0201	// #ae_WeightTbl_16_5_
	{0x0F12, 0x0000},	//0102	// #ae_WeightTbl_16_6_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_7_
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_8_
	{0x0F12, 0x0101},	//0202	// #ae_WeightTbl_16_9_
	{0x0F12, 0x0101},	//0202	// #ae_WeightTbl_16_10
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_11
	{0x0F12, 0x0201},	//0101	// #ae_WeightTbl_16_12
	{0x0F12, 0x0202},	//0402	// #ae_WeightTbl_16_13
	{0x0F12, 0x0102},	//0204	// #ae_WeightTbl_16_14
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_15
	{0x0F12, 0x0201},	//0201	// #ae_WeightTbl_16_16
	{0x0F12, 0x0505},	//0504	// #ae_WeightTbl_16_17
	{0x0F12, 0x0102},	//0405	// #ae_WeightTbl_16_18
	{0x0F12, 0x0000},	//0102	// #ae_WeightTbl_16_19
	{0x0F12, 0x0201},	//0501	// #ae_WeightTbl_16_20
	{0x0F12, 0x0505},	//0505	// #ae_WeightTbl_16_21
	{0x0F12, 0x0102},	//0505	// #ae_WeightTbl_16_22
	{0x0F12, 0x0000},	//0105	// #ae_WeightTbl_16_23
	{0x0F12, 0x0201},	//0503	// #ae_WeightTbl_16_24
	{0x0F12, 0x0202},	//0505	// #ae_WeightTbl_16_25
	{0x0F12, 0x0102},	//0505	// #ae_WeightTbl_16_26
	{0x0F12, 0x0000},	//0305	// #ae_WeightTbl_16_27
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_28
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_29
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_30
	{0x0F12, 0x0000},	//0303	// #ae_WeightTbl_16_31



	//=================================================================================================
	//	Set GAS & CCM White Point
	//=================================================================================================
	// param_start	TVAR_ash_AwbAshCord
	{0x002A, 0x0704},
	{0x0F12, 0x0090},	//000E	// #TVAR_ash_AwbAshCord_0_
	{0x0F12, 0x00B3},	//0012	// #TVAR_ash_AwbAshCord_1_
	{0x0F12, 0x00C3},	//0019	// #TVAR_ash_AwbAshCord_2_
	{0x0F12, 0x00E5},	//0034	// #TVAR_ash_AwbAshCord_3_
	{0x0F12, 0x0116},	//0044	// #TVAR_ash_AwbAshCord_4_
	{0x0F12, 0x0198},	//0052	// #TVAR_ash_AwbAshCord_5_
	{0x0F12, 0x0200},	//0058	// #TVAR_ash_AwbAshCord_6_

	// param_start	wbt_AwbCcmCord
	{0x002A, 0x06F2},
	{0x0F12, 0x00B3},	//0016	// #SARR_AwbCcmCord_0_	Hor
	{0x0F12, 0x00E5},	//001E	// #SARR_AwbCcmCord_1_	IncaA
	{0x0F12, 0x0100},	//0024	// #SARR_AwbCcmCord_2_	WW
	{0x0F12, 0x0116},	//0038	// #SARR_AwbCcmCord_3_	CW
	{0x0F12, 0x0150},	//004C	// #SARR_AwbCcmCord_4_	D50
	{0x0F12, 0x0198},	//0058	// #SARR_AwbCcmCord_5_	D65

	// Set Target Brightness Control
	{0x002A, 0x103E},
	{0x0F12, 0x0000},	// #SARR_IllumType_0_
	{0x0F12, 0x0009},	// #SARR_IllumType_1_
	{0x0F12, 0x0018},	// #SARR_IllumType_2_
	{0x0F12, 0x0032},	// #SARR_IllumType_3_
	{0x0F12, 0x004A},	// #SARR_IllumType_4_
	{0x0F12, 0x0051},	// #SARR_IllumType_5_
	{0x0F12, 0x0056},	// #SARR_IllumType_6_
	{0x0F12, 0x010C},	// #SARe_2_R_IllumTypeF_0_
	{0x0F12, 0x010C},	// #SARe_3_R_IllumTypeF_1_
	{0x0F12, 0x0109},	// #SARe_4_R_IllumTypeF_2_
	{0x0F12, 0x0105},	// #SARe_5_R_IllumTypeF_3_
	{0x0F12, 0x0102},	// #SARe_6_R_IllumTypeF_4_
	{0x0F12, 0x00FB},	// #SARR_IllumTypeF_5_
	{0x0F12, 0x00F8},	// #SARR_IllumTypeF_6_

	// Indoor GAS Alpha
	{0x002A, 0x0712},
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[0]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[1]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[2]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[3]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[4]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[5]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[6]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[7]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[8]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[9]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[10]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[11]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[12]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[13]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[14]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[15]
	{0x0F12, 0x00E0},	// #TVAR_ash_GASAlpha[16]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[17]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[18]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[19]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[20]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[21]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[22]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[23]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[24]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[25]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[26]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[27]

	// Outdoor GAS Alpha
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_0_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_1_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_2_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_3_

	// GAS LUT Start Address
	{0x002A, 0x0754},
	{0x0F12, 0x2388},	//#TVAR_ash_pGAS
	{0x0F12, 0x7000},	//#TVAR_ash_pGAS

	// GAS LUT
	//							//1030	//1027
	{0x002A, 0x2388},
	{0x0F12, 0x01C9},	//#TVAR_ash_pGAS[0]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[1]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[2]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[3]
	{0x0F12, 0x0105},	//#TVAR_ash_pGAS[4]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[5]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[6]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[7]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[8]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[9]
	{0x0F12, 0x0160},	//#TVAR_ash_pGAS[10]
	{0x0F12, 0x0189},	//#TVAR_ash_pGAS[11]
	{0x0F12, 0x01D0},	//#TVAR_ash_pGAS[12]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[13]
	{0x0F12, 0x016A},	//#TVAR_ash_pGAS[14]
	{0x0F12, 0x0126},	//#TVAR_ash_pGAS[15]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[16]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[17]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[18]
	{0x0F12, 0x009B},	//#TVAR_ash_pGAS[19]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[20]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[21]
	{0x0F12, 0x00EF},	//#TVAR_ash_pGAS[22]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[23]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[24]
	{0x0F12, 0x019C},	//#TVAR_ash_pGAS[25]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[26]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[27]
	{0x0F12, 0x00E2},	//#TVAR_ash_pGAS[28]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[29]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[30]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[31]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[32]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[33]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[34]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[35]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[36]
	{0x0F12, 0x0133},	//#TVAR_ash_pGAS[37]
	{0x0F12, 0x016E},	//#TVAR_ash_pGAS[38]
	{0x0F12, 0x0140},	//#TVAR_ash_pGAS[39]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[40]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[41]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[42]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[43]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[44]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[45]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[46]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[47]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[48]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[49]
	{0x0F12, 0x010D},	//#TVAR_ash_pGAS[50]
	{0x0F12, 0x0157},	//#TVAR_ash_pGAS[51]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[52]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[53]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[54]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[55]
	{0x0F12, 0x002F},	//#TVAR_ash_pGAS[56]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[57]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[58]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[59]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[60]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[61]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[62]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[63]
	{0x0F12, 0x0142},	//#TVAR_ash_pGAS[64]
	{0x0F12, 0x011A},	//#TVAR_ash_pGAS[65]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[66]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[67]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[68]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[69]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[70]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[71]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[72]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[73]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[74]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[75]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[76]
	{0x0F12, 0x013B},	//#TVAR_ash_pGAS[77]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[78]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[79]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[80]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[81]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[82]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[83]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[84]
	{0x0F12, 0x0014},	//#TVAR_ash_pGAS[85]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[86]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[87]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[88]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[89]
	{0x0F12, 0x013F},	//#TVAR_ash_pGAS[90]
	{0x0F12, 0x012C},	//#TVAR_ash_pGAS[91]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[92]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[93]
	{0x0F12, 0x0067},	//#TVAR_ash_pGAS[94]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[95]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[96]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[97]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[98]
	{0x0F12, 0x004B},	//#TVAR_ash_pGAS[99]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[100]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[101]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[102]
	{0x0F12, 0x0150},	//#TVAR_ash_pGAS[103]
	{0x0F12, 0x0149},	//#TVAR_ash_pGAS[104]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[105]
	{0x0F12, 0x00C5},	//#TVAR_ash_pGAS[106]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[107]
	{0x0F12, 0x0061},	//#TVAR_ash_pGAS[108]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[109]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[110]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[111]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[112]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[113]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[114]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[115]
	{0x0F12, 0x016D},	//#TVAR_ash_pGAS[116]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[117]
	{0x0F12, 0x013A},	//#TVAR_ash_pGAS[118]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[119]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[120]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[121]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[122]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[123]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[124]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[125]
	{0x0F12, 0x00D8},	//#TVAR_ash_pGAS[126]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[127]
	{0x0F12, 0x0154},	//#TVAR_ash_pGAS[128]
	{0x0F12, 0x0186},	//#TVAR_ash_pGAS[129]
	{0x0F12, 0x018F},	//#TVAR_ash_pGAS[130]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[131]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[132]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[133]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[134]
	{0x0F12, 0x00B4},	//#TVAR_ash_pGAS[135]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[136]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[137]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[138]
	{0x0F12, 0x0112},	//#TVAR_ash_pGAS[139]
	{0x0F12, 0x014F},	//#TVAR_ash_pGAS[140]
	{0x0F12, 0x0185},	//#TVAR_ash_pGAS[141]
	{0x0F12, 0x01A1},	//#TVAR_ash_pGAS[142]
	{0x0F12, 0x0168},	//#TVAR_ash_pGAS[143]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[144]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[145]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[146]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[147]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[148]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[149]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[150]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[151]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[152]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[153]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[154]
	{0x0F12, 0x0163},	//#TVAR_ash_pGAS[155]
	{0x0F12, 0x0131},	//#TVAR_ash_pGAS[156]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[157]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[158]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[159]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[160]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[161]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[162]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[163]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[164]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[165]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[166]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[167]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[168]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[169]
	{0x0F12, 0x00D2},	//#TVAR_ash_pGAS[170]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[171]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[172]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[173]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[174]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[175]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[176]
	{0x0F12, 0x0059},	//#TVAR_ash_pGAS[177]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[178]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[179]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[180]
	{0x0F12, 0x010C},	//#TVAR_ash_pGAS[181]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[182]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[183]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[184]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[185]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[186]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[187]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[188]
	{0x0F12, 0x0025},	//#TVAR_ash_pGAS[189]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[190]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[191]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[192]
	{0x0F12, 0x00C2},	//#TVAR_ash_pGAS[193]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[194]
	{0x0F12, 0x00DD},	//#TVAR_ash_pGAS[195]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[196]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[197]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[198]
	{0x0F12, 0x001E},	//#TVAR_ash_pGAS[199]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[200]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[201]
	{0x0F12, 0x0010},	//#TVAR_ash_pGAS[202]
	{0x0F12, 0x0026},	//#TVAR_ash_pGAS[203]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[204]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[205]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[206]
	{0x0F12, 0x00EC},	//#TVAR_ash_pGAS[207]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[208]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[209]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[210]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[211]
	{0x0F12, 0x0017},	//#TVAR_ash_pGAS[212]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[213]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[214]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[215]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[216]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[217]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[218]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[219]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[220]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[221]
	{0x0F12, 0x009A},	//#TVAR_ash_pGAS[222]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[223]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[224]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[225]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[226]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[227]
	{0x0F12, 0x000F},	//#TVAR_ash_pGAS[228]
	{0x0F12, 0x0027},	//#TVAR_ash_pGAS[229]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[230]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[231]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[232]
	{0x0F12, 0x00F1},	//#TVAR_ash_pGAS[233]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[234]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[235]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[236]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[237]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[238]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[239]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[240]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[241]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[242]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[243]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[244]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[245]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[246]
	{0x0F12, 0x00FA},	//#TVAR_ash_pGAS[247]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[248]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[249]
	{0x0F12, 0x0064},	//#TVAR_ash_pGAS[250]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[251]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[252]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[253]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[254]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[255]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[256]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[257]
	{0x0F12, 0x00DF},	//#TVAR_ash_pGAS[258]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[259]
	{0x0F12, 0x0110},	//#TVAR_ash_pGAS[260]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[261]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[262]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[263]
	{0x0F12, 0x006E},	//#TVAR_ash_pGAS[264]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[265]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[266]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[267]
	{0x0F12, 0x0083},	//#TVAR_ash_pGAS[268]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[269]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[270]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[271]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[272]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[273]
	{0x0F12, 0x0104},	//#TVAR_ash_pGAS[274]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[275]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[276]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[277]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[278]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[279]
	{0x0F12, 0x0098},	//#TVAR_ash_pGAS[280]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[281]
	{0x0F12, 0x00D1},	//#TVAR_ash_pGAS[282]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[283]
	{0x0F12, 0x0127},	//#TVAR_ash_pGAS[284]
	{0x0F12, 0x0146},	//#TVAR_ash_pGAS[285]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[286]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[287]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[288]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[289]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[290]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[291]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[292]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[293]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[294]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[295]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[296]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[297]
	{0x0F12, 0x0180},	//#TVAR_ash_pGAS[298]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[299]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[300]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[301]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[302]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[303]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[304]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[305]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[306]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[307]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[308]
	{0x0F12, 0x00EE},	//#TVAR_ash_pGAS[309]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[310]
	{0x0F12, 0x014C},	//#TVAR_ash_pGAS[311]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[312]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[313]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[314]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[315]
	{0x0F12, 0x0057},	//#TVAR_ash_pGAS[316]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[317]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[318]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[319]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[320]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[321]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[322]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[323]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[324]
	{0x0F12, 0x00F6},	//#TVAR_ash_pGAS[325]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[326]
	{0x0F12, 0x0081},	//#TVAR_ash_pGAS[327]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[328]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[329]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[330]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[331]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[332]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[333]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[334]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[335]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[336]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[337]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[338]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[339]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[340]
	{0x0F12, 0x0041},	//#TVAR_ash_pGAS[341]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[342]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[343]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[344]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[345]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[346]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[347]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[348]
	{0x0F12, 0x00BA},	//#TVAR_ash_pGAS[349]
	{0x0F12, 0x00F7},	//#TVAR_ash_pGAS[350]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[351]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[352]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[353]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[354]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[355]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[356]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[357]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[358]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[359]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[360]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[361]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[362]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[363]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[364]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[365]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[366]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[367]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[368]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[369]
	{0x0F12, 0x0002},	//#TVAR_ash_pGAS[370]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[371]
	{0x0F12, 0x0023},	//#TVAR_ash_pGAS[372]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[373]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[374]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[375]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[376]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[377]
	{0x0F12, 0x00B0},	//#TVAR_ash_pGAS[378]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[379]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[380]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[381]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[382]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[383]
	{0x0F12, 0x001C},	//#TVAR_ash_pGAS[384]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[385]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[386]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[387]
	{0x0F12, 0x00B7},	//#TVAR_ash_pGAS[388]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[389]
	{0x0F12, 0x0106},	//#TVAR_ash_pGAS[390]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[391]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[392]
	{0x0F12, 0x006C},	//#TVAR_ash_pGAS[393]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[394]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[395]
	{0x0F12, 0x0033},	//#TVAR_ash_pGAS[396]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[397]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[398]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[399]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[400]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[401]
	{0x0F12, 0x0101},	//#TVAR_ash_pGAS[402]
	{0x0F12, 0x0120},	//#TVAR_ash_pGAS[403]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[404]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[405]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[406]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[407]
	{0x0F12, 0x0062},	//#TVAR_ash_pGAS[408]
	{0x0F12, 0x005C},	//#TVAR_ash_pGAS[409]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[410]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[411]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[412]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[413]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[414]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[415]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[416]
	{0x0F12, 0x0115},	//#TVAR_ash_pGAS[417]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[418]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[419]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[420]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[421]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[422]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[423]
	{0x0F12, 0x00A4},	//#TVAR_ash_pGAS[424]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[425]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[426]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[427]
	{0x0F12, 0x0130},	//#TVAR_ash_pGAS[428]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[429]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[430]
	{0x0F12, 0x00C6},	//#TVAR_ash_pGAS[431]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[432]
	{0x0F12, 0x0090},	//#TVAR_ash_pGAS[433]
	{0x0F12, 0x0085},	//#TVAR_ash_pGAS[434]
	{0x0F12, 0x0082},	//#TVAR_ash_pGAS[435]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[436]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[437]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[438]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[439]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[440]
	{0x0F12, 0x0135},	//#TVAR_ash_pGAS[441]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[442]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[443]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[444]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[445]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[446]
	{0x0F12, 0x005D},	//#TVAR_ash_pGAS[447]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[448]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[449]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[450]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[451]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[452]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[453]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[454]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[455]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[456]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[457]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[458]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[459]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[460]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[461]
	{0x0F12, 0x003D},	//#TVAR_ash_pGAS[462]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[463]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[464]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[465]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[466]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[467]
	{0x0F12, 0x00B5},	//#TVAR_ash_pGAS[468]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[469]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[470]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[471]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[472]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[473]
	{0x0F12, 0x001A},	//#TVAR_ash_pGAS[474]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[475]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[476]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[477]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[478]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[479]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[480]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[481]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[482]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[483]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[484]
	{0x0F12, 0x0018},	//#TVAR_ash_pGAS[485]
	{0x0F12, 0x000A},	//#TVAR_ash_pGAS[486]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[487]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[488]
	{0x0F12, 0x001F},	//#TVAR_ash_pGAS[489]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[490]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[491]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[492]
	{0x0F12, 0x00B6},	//#TVAR_ash_pGAS[493]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[494]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[495]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[496]
	{0x0F12, 0x0028},	//#TVAR_ash_pGAS[497]
	{0x0F12, 0x0012},	//#TVAR_ash_pGAS[498]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[499]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[500]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[501]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[502]
	{0x0F12, 0x0030},	//#TVAR_ash_pGAS[503]
	{0x0F12, 0x0050},	//#TVAR_ash_pGAS[504]
	{0x0F12, 0x007B},	//#TVAR_ash_pGAS[505]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[506]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[507]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[508]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[509]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[510]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[511]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[512]
	{0x0F12, 0x0001},	//#TVAR_ash_pGAS[513]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[514]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[515]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[516]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[517]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[518]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[519]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[520]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[521]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[522]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[523]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[524]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[525]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[526]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[527]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[528]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[529]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[530]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[531]
	{0x0F12, 0x00AE},	//#TVAR_ash_pGAS[532]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[533]
	{0x0F12, 0x0097},	//#TVAR_ash_pGAS[534]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[535]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[536]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[537]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[538]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[539]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[540]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[541]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[542]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[543]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[544]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[545]
	{0x0F12, 0x00D3},	//#TVAR_ash_pGAS[546]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[547]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[548]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[549]
	{0x0F12, 0x0058},	//#TVAR_ash_pGAS[550]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[551]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[552]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[553]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[554]
	{0x0F12, 0x006D},	//#TVAR_ash_pGAS[555]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[556]
	{0x0F12, 0x00AC},	//#TVAR_ash_pGAS[557]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[558]
	{0x0F12, 0x00F5},	//#TVAR_ash_pGAS[559]
	{0x0F12, 0x00D4},	//#TVAR_ash_pGAS[560]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[561]
	{0x0F12, 0x008E},	//#TVAR_ash_pGAS[562]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[563]
	{0x0F12, 0x0070},	//#TVAR_ash_pGAS[564]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[565]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[566]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[567]
	{0x0F12, 0x0093},	//#TVAR_ash_pGAS[568]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[569]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[570]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[571]



	//=================================================================================================
	//	Set Gamma LUT
	//=================================================================================================
	//							//New	//Hong	//ORG
	{0x002A, 0x04CC},
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[0][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[0][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[0][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[0][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[0][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[0][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[0][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[0][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[0][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[0][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[0][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[0][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[0][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[0][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[0][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[0][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[1][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[1][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[1][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[1][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[1][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[1][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[1][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[1][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[1][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[1][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[1][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[1][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[1][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[1][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[1][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[1][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[2][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[2][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[2][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[2][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[2][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[2][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[2][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[2][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[2][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[2][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[2][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[2][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[2][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[2][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[2][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[2][15]



	//=================================================================================================
	//	Set AWB
	//=================================================================================================
	{0x002A, 0x0DA8},
	{0x0F12, 0x0020},	// #awbb_LowBr0_NBzone	// 2008.04.24. eliminate to the purple color at low light.
	{0x002A, 0x0E8C},
	{0x0F12, 0x0000},	// #awbb_LowBr0_PatchNumZone
	{0x002A, 0x0D6C},
	{0x0F12, 0x0040},	// #awbb_YMedMoveToYAv

	// Indoor Gray Zone
	{0x002A, 0x0B9C},
	{0x0F12, 0x0339},	//0392	// #awbb_IndoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x0393},	//03A4	// #awbb_IndoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x031B},	//036F	// #awbb_IndoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x039C},	//03AE	// #awbb_IndoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x030A},	//034E	// #awbb_IndoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0398},	//03A2	// #awbb_IndoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x02F3},	//0331	// #awbb_IndoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0392},	//0392	// #awbb_IndoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x02E0},	//0315	// #awbb_IndoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0388},	//0381	// #awbb_IndoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x02CB},	//02F8	// #awbb_IndoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0374},	//036C	// #awbb_IndoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x02BB},	//02E1	// #awbb_IndoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x034B},	//0358	// #awbb_IndoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x02AA},	//02C5	// #awbb_IndoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x0336},	//033B	// #awbb_IndoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x029C},	//02B1	// #awbb_IndoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0321},	//0323	// #awbb_IndoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x028B},	//029C	// #awbb_IndoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x030B},	//030F	// #awbb_IndoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x027B},	//028B	// #awbb_IndoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02FD},	//02FA	// #awbb_IndoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x0268},	//027C	// #awbb_IndoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x02EA},	//02E2	// #awbb_IndoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x0259},	//0268	// #awbb_IndoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x02D8},	//02D1	// #awbb_IndoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x024C},	//0259	// #awbb_IndoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x02CB},	//02BF	// #awbb_IndoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x023D},	//024C	// #awbb_IndoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x02BD},	//02B1	// #awbb_IndoorGrZones_m_BGrid_14__m_right
	{0x0F12, 0x0231},	//023D	// #awbb_IndoorGrZones_m_BGrid_15__m_left
	{0x0F12, 0x02B1},	//02A2	// #awbb_IndoorGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0222},	//0231	// #awbb_IndoorGrZones_m_BGrid_16__m_left
	{0x0F12, 0x02A8},	//0299	// #awbb_IndoorGrZones_m_BGrid_16__m_right
	{0x0F12, 0x0215},	//0222	// #awbb_IndoorGrZones_m_BGrid_17__m_left
	{0x0F12, 0x029B},	//0290	// #awbb_IndoorGrZones_m_BGrid_17__m_right
	{0x0F12, 0x0208},	//0215	// #awbb_IndoorGrZones_m_BGrid_18__m_left
	{0x0F12, 0x0290},	//0288	// #awbb_IndoorGrZones_m_BGrid_18__m_right
	{0x0F12, 0x01F7},	//0208	// #awbb_IndoorGrZones_m_BGrid_19__m_left
	{0x0F12, 0x0287},	//0281	// #awbb_IndoorGrZones_m_BGrid_19__m_right
	{0x0F12, 0x01E6},	//01F7	// #awbb_IndoorGrZones_m_BGrid_20__m_left
	{0x0F12, 0x027F},	//0279	// #awbb_IndoorGrZones_m_BGrid_20__m_right
	{0x0F12, 0x01D9},	//01E6	// #awbb_IndoorGrZones_m_BGrid_21__m_left
	{0x0F12, 0x0279},	//026E	// #awbb_IndoorGrZones_m_BGrid_21__m_right
	{0x0F12, 0x01CC},	//01D9	// #awbb_IndoorGrZones_m_BGrid_22__m_left
	{0x0F12, 0x0272},	//0268	// #awbb_IndoorGrZones_m_BGrid_22__m_right
	{0x0F12, 0x01C1},	//01CC	// #awbb_IndoorGrZones_m_BGrid_23__m_left
	{0x0F12, 0x026C},	//0263	// #awbb_IndoorGrZones_m_BGrid_23__m_right
	{0x0F12, 0x01B3},	//01C1	// #awbb_IndoorGrZones_m_BGrid_24__m_left
	{0x0F12, 0x0265},	//0260	// #awbb_IndoorGrZones_m_BGrid_24__m_right
	{0x0F12, 0x01AA},	//01B3	// #awbb_IndoorGrZones_m_BGrid_25__m_left
	{0x0F12, 0x025C},	//0259	// #awbb_IndoorGrZones_m_BGrid_25__m_right
	{0x0F12, 0x019F},	//01AA	// #awbb_IndoorGrZones_m_BGrid_26__m_left
	{0x0F12, 0x0250},	//0253	// #awbb_IndoorGrZones_m_BGrid_26__m_right
	{0x0F12, 0x019B},	//019F	// #awbb_IndoorGrZones_m_BGrid_27__m_left
	{0x0F12, 0x0247},	//0248	// #awbb_IndoorGrZones_m_BGrid_27__m_right
	{0x0F12, 0x0197},	//019B	// #awbb_IndoorGrZones_m_BGrid_28__m_left
	{0x0F12, 0x0239},	//023F	// #awbb_IndoorGrZones_m_BGrid_28__m_right
	{0x0F12, 0x019B},	//0197	// #awbb_IndoorGrZones_m_BGrid_29__m_left
	{0x0F12, 0x021D},	//0239	// #awbb_IndoorGrZones_m_BGrid_29__m_right
	{0x0F12, 0x019D},	//019B	// #awbb_IndoorGrZones_m_BGrid_30__m_left
	{0x0F12, 0x0202},	//021D	// #awbb_IndoorGrZones_m_BGrid_30__m_right
	{0x0F12, 0x01AC},	//019D	// #awbb_IndoorGrZones_m_BGrid_31__m_left
	{0x0F12, 0x01D3},	//020A	// #awbb_IndoorGrZones_m_BGrid_31__m_right
	{0x0F12, 0x0000},	//01AC	// #awbb_IndoorGrZones_m_BGrid_32__m_left
	{0x0F12, 0x0000},	//01D3	// #awbb_IndoorGrZones_m_BGrid_32__m_right
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_left
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_right

	{0x002A, 0x0C3C},
	{0x0F12, 0x0004},	// #awbb_IndoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0138},	// #awbb_IndoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x0F12, 0x0000},

	// Outdoor Gray Zone
	{0x0F12, 0x0255},	//#awbb_OutdoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x0236},	//#awbb_OutdoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x0290},	//#awbb_OutdoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x0227},	//#awbb_OutdoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0297},	//#awbb_OutdoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x0210},	//#awbb_OutdoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0293},	//#awbb_OutdoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0200},	//#awbb_OutdoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x028B},	//#awbb_OutdoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x01F1},	//#awbb_OutdoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0286},	//#awbb_OutdoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x0282},	//#awbb_OutdoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x01E0},	//#awbb_OutdoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x027B},	//#awbb_OutdoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x01DE},	//#awbb_OutdoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0274},	//#awbb_OutdoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01DF},	//#awbb_OutdoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x0263},	//#awbb_OutdoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01F6},	//#awbb_OutdoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x0253},	//#awbb_OutdoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x020B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x023B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_right

	{0x002A, 0x0CB8},
	{0x0F12, 0x0004},	// #awbb_OutdoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x000C},	//#awbb_OutdoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0217},	//#awbb_OutdoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high
	{0x0F12, 0x0000},

	// Low Brightness Gray Zone
	{0x0F12, 0x0324},	//031E  // #awbb_LowBrGrZones_m_BGrid_0__m_left
	{0x0F12, 0x03A1},	//037E  // #awbb_LowBrGrZones_m_BGrid_0__m_right
	{0x0F12, 0x02F2},	//02CF  // #awbb_LowBrGrZones_m_BGrid_1__m_left
	{0x0F12, 0x03BA},	//0389  // #awbb_LowBrGrZones_m_BGrid_1__m_right
	{0x0F12, 0x02C4},	//02A6  // #awbb_LowBrGrZones_m_BGrid_2__m_left
	{0x0F12, 0x03B6},	//0386  // #awbb_LowBrGrZones_m_BGrid_2__m_right
	{0x0F12, 0x029E},	//0282  // #awbb_LowBrGrZones_m_BGrid_3__m_left
	{0x0F12, 0x03A6},	//0374  // #awbb_LowBrGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0283},	//0259  // #awbb_LowBrGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0380},	//0352  // #awbb_LowBrGrZones_m_BGrid_4__m_right
	{0x0F12, 0x025F},	//0239  // #awbb_LowBrGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0344},	//0330  // #awbb_LowBrGrZones_m_BGrid_5__m_right
	{0x0F12, 0x0241},	//021E  // #awbb_LowBrGrZones_m_BGrid_6__m_left
	{0x0F12, 0x031F},	//031B  // #awbb_LowBrGrZones_m_BGrid_6__m_right
	{0x0F12, 0x0227},	//020B  // #awbb_LowBrGrZones_m_BGrid_7__m_left
	{0x0F12, 0x02FC},	//0300  // #awbb_LowBrGrZones_m_BGrid_7__m_right
	{0x0F12, 0x020A},	//01F1  // #awbb_LowBrGrZones_m_BGrid_8__m_left
	{0x0F12, 0x02D9},	//02DD  // #awbb_LowBrGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01F1},	//01E0  // #awbb_LowBrGrZones_m_BGrid_9__m_left
	{0x0F12, 0x02C4},	//02C4  // #awbb_LowBrGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01D0},	//01CD  // #awbb_LowBrGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02B2},	//02AF  // #awbb_LowBrGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01B8},	//019E  // #awbb_LowBrGrZones_m_BGrid_11__m_left
	{0x0F12, 0x029E},	//02A0  // #awbb_LowBrGrZones_m_BGrid_11__m_right
	{0x0F12, 0x019F},	//0187  // #awbb_LowBrGrZones_m_BGrid_12__m_left
	{0x0F12, 0x0290},	//028C  // #awbb_LowBrGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0189},	//017D  // #awbb_LowBrGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0282},	//027E  // #awbb_LowBrGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0182},	//0179  // #awbb_LowBrGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0271},	//0258  // #awbb_LowBrGrZones_m_BGrid_14__m_right
	{0x0F12, 0x017F},	//0186  // #awbb_LowBrGrZones_m_BGrid_15__m_left
	{0x0F12, 0x0254},	//0232  // #awbb_LowBrGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0183},	//01BD  // #awbb_LowBrGrZones_m_BGrid_16__m_left
	{0x0F12, 0x022D},	//01FB  // #awbb_LowBrGrZones_m_BGrid_16__m_right
	{0x0F12, 0x019C},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_left
	{0x0F12, 0x01D3},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_right

	{0x002A, 0x0D34},
	{0x0F12, 0x0005},	// #awbb_LowBrGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0012},	// #awbb_LowBrGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0120},	// #awbb_LowBrGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0002},	// #awbb_LowBrGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D48},
	{0x0F12, 0x00E0},	// #awbb_LowBrGrZones_y_high
	{0x0F12, 0x0000},

	// Lowtemp Circle
	{0x0F12, 0x032F},	// #awbb_CrclLowT_R_c
	{0x0F12, 0x0000},
	{0x0F12, 0x017A},	// #awbb_CrclLowT_B_c
	{0x0F12, 0x0000},
	{0x0F12, 0x7300},	// #awbb_CrclLowT_Rad_c
	{0x0F12, 0x0000},
	{0x0F12, 0x000A},	// #awbb_CrclLowT_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D60},
	{0x0F12, 0x00E0},	// #awbb_CrclLowT_y_high
	{0x0F12, 0x0000},
	{0x002A, 0x0D82},
	{0x0F12, 0x0001},	// #awbb_ByPass_LowTempMode

	// White Locus
	{0x002A, 0x0D66},
	{0x0F12, 0x011F},	// #awbb_IntcR
	{0x0F12, 0x0132},	// #awbb_IntcB
	{0x002A, 0x0D74},
	{0x0F12, 0x052A},	// #awbb_MvEq_RBthresh

	// Gamut Thresholds
	{0x002A, 0x0DAE},
	{0x0F12, 0x0036},	// #awbb_GamutWidthThr2
	{0x0F12, 0x001C},	// #awbb_GamutHeightThr2
	{0x002A, 0x0DAA},
	{0x0F12, 0x071A},	// #awbb_GamutWidthThr1
	{0x0F12, 0x03A4},	// #awbb_GamutHeightThr1

	// SceneDetection Thresholds
	{0x002A, 0x0D92},
	{0x0F12, 0x0CE4},	//0A8C	// #awbb_SunnyBr
	{0x0F12, 0x0096},	// #awbb_Sunny_NBzone
	{0x002A, 0x0E86},
	{0x0F12, 0x0216},	// #awbb_OutdoorWP_r
	{0x0F12, 0x029F},	// #awbb_OutdoorWP_b
	{0x002A, 0x0D96},
	{0x0F12, 0x0580},	// #awbb_CloudyBr
	{0x0F12, 0x0096},	// #awbb_Cloudy_NBzone
	{0x002A, 0x0DB2},
	{0x0F12, 0x00DA},	// #awbb_CloudyRB
	{0x002A, 0x0D9A},
	{0x0F12, 0x000A},	// #awbb_Cloudy_BdivRzone
	{0x002A, 0x0DB4},
	{0x0F12, 0x0459},	// #awbb_LowTempRB
	{0x002A, 0x0DA4},
	{0x0F12, 0x000E},	// #awbb_LowTemp_RBzone
	{0x002A, 0x0D64},
	{0x0F12, 0x0040},	// #awbb_DarkBr
	{0x002A, 0x0DA6},
	{0x0F12, 0x001E},	// #awbb_LowBr_NBzone
	{0x002A, 0x0D9C},
	{0x0F12, 0x001B},	// #awbb_MacbethGamut_WidthZone
	{0x0F12, 0x000E},	// #awbb_MacbethGamut_HeightZone
	{0x0F12, 0x0008},	// #awbb_MacbethGamut_WidthZone2
	{0x0F12, 0x0004},	// #awbb_MacbethGamut_HeightZone2

	// AWB Debug.(Outdoor Pink)
	{0x002A, 0x0E30},
	{0x0F12, 0x0000},	//	#awbb_OutdoorFltrSz (outdoor WB를 moving average filtering)

	// UseInvalidOutdoor option
	{0x002A, 0x0D88},
	{0x0F12, 0x0001},	// #awbb_Use_InvalidOutDoor

	// AWB input Y-Filter setting
	{0x002A, 0x0C48},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x002A, 0x0CC4},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high

	{0x002A, 0x0DC2},
	{0x0F12, 0x0030},	// #awbb_GnCurPntImmunity
	{0x0F12, 0x00C8},	// #awbb_GnFarPntImmunity
	{0x0F12, 0x012C},	// #awbb_GnCurPntLongJump
	{0x0F12, 0x0258},	// #awbb_GainsMaxMove
	{0x0F12, 0x0003},	// #awbb_GnMinMatchToJump

	// RB Gain Offset
	//WRITE 70000E32	FFE0	//#awbb_RGainOff
	//WRITE 70000E34	FFe0	//#awbb_BGainOff


	//=================================================================================================
	//	Set CCM
	//=================================================================================================
	// CCM Start Address
	{0x002A, 0x06D0},
	{0x0F12, 0x2800},   //#TVAR_wbt_pBaseCcmsAddr[0]
	{0x0F12, 0x7000},
	{0x0F12, 0x2824},   //#TVAR_wbt_pBaseCcmsAddr[1]
	{0x0F12, 0x7000},
	{0x0F12, 0x2848},   //#TVAR_wbt_pBaseCcmsAddr[2]
	{0x0F12, 0x7000},
	{0x0F12, 0x286C},   //#TVAR_wbt_pBaseCcmsAddr[3]
	{0x0F12, 0x7000},
	{0x0F12, 0x2890},   //#TVAR_wbt_pBaseCcmsAddr[4]
	{0x0F12, 0x7000},
	{0x0F12, 0x28B4},   //#TVAR_wbt_pBaseCcmsAddr[5]
	{0x0F12, 0x7000},
	{0x002A, 0x06EC},
	{0x0F12, 0x28D8},   //#TVAR_wbt_pOutdoorCcm
	{0x0F12, 0x7000},

	{0x002A, 0x2800},
	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	// Outdoor CCM
	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEA4},
	{0x0F12, 0x020F},
	{0x0F12, 0xFF8F},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},



	//=================================================================================================
	//	Set NB
	//=================================================================================================
	{0x002A, 0x07EA},
	{0x0F12, 0x0000},	//afit_bUseNoiseInd 0 : NB 1: Noise Index

	// param_start	SARR_uNormBrInDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrInDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrInDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrInDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrInDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrInDoor[4]

	// param_start	SARR_uNormBrOutDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrOutDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrOutDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrOutDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrOutDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrOutDoor[4]



	//=================================================================================================
	//	Set AFIT
	//=================================================================================================
	// AFIT Start Address
	{0x002A, 0x0814},
	{0x0F12, 0x082C},	//#TVAR_afit_pBaseVals
	{0x0F12, 0x7000},	//#TVAR_afit_pBaseVals

	// AFIT Table	//0223
	{0x002A, 0x082C},
	{0x0F12, 0x0010},	//BRIGHTNESS
	{0x0F12, 0xFFF0},	//CONTRAST
	{0x0F12, 0xFFD0},	//SATURATION
	{0x0F12, 0x0000},	//SHARP_BLUR
	{0x0F12, 0x0000},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x003B},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                   bin: desparity low
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"                bin: desparity high
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x06FF},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x030A},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"               ]negati fine
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"                  ]low fine
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"                ]high fine
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"                   ]high low thres
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0005},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01AA},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0006},	//BRIGHTNESS
	{0x0F12, 0x0000},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0000},	//SHARP_BLUR
	{0x0F12, 0x0000},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0021},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                  bin: desparity low
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x08B7},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0308},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01A0},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0021},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0089},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0202},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0202},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0206},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x803C},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0019},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0205},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0000},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8050},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0019},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x005A},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0000},	//UVDenoise_iYLowThresh
	{0x0F12, 0x0000},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x3202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x3202},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0105},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0200},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x805A},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[0]
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[1]
	{0x0F12, 0x0800},	// # afit_pConstBaseVals[2]
	{0x0F12, 0x0600},	// # afit_pConstBaseVals[3]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[4]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[5]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[6]
	{0x0F12, 0x0300},	// # afit_pConstBaseVals[7]
	{0x0F12, 0x0002},	// # afit_pConstBaseVals[8]
	{0x0F12, 0x0400},	// # afit_pConstBaseVals[9]
	{0x0F12, 0x0106},	// # afit_pConstBaseVals[10]
	{0x0F12, 0x0005},	// # afit_pConstBaseVals[11]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[12]
	{0x0F12, 0x0003},	// # afit_pConstBaseVals[13]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[14]
	{0x0F12, 0xFFD6},	// # afit_pConstBaseVals[15]
	{0x0F12, 0x53C1},	// # afit_pConstBaseVals[16]
	{0x0F12, 0xE1FE},	// # afit_pConstBaseVals[17]
	{0x0F12, 0x0001},	// # afit_pConstBaseVals[18]

	{0x002A, 0x01EA},
	{0x0F12, 0x0000},	//#REG_TC_UserSharpBlur
	{0x002A, 0x0E32},
	{0x0F12, 0xFFF0},	//#awbb_RGainOff
	{0x0F12, 0x0000},	//#awbb_BGainOff

	// For fast AE
	{0x002A, 0x0FAC},
	{0x0F12, 0x0001},	// AE Gain output [0]    default : 0x10
	{0x0F12, 0x0008},	// AE Gain output [1]    default : 0x20
	{0x0F12, 0x0015},	// AE Gain output [2]    default : 0x40

	// For fast AWB
	{0x002A, 0x0DCA},
	{0x0F12, 0x0001},	// awbb_GnMinMatchToJump = 0x02 (default:0x03)
	{0x0028, 0x7000},
	{0x002A, 0x0E98},
	{0x0F12, 0x0300},	//bp_uMaxBrightnessFactor 0180 -> 0300
	{0x002A, 0x0E9E},
	{0x0F12, 0x0300},	//bp_uMinBrightnessFactor 0180 -> 0300

	// Update Changed Registers
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},

	{0x0028, 0xD000},
	{0x002A, 0x1088},
	{0x0F12, 0x05D5},

	{0x0028, 0xD000},
	{0x002A, 0x1094},
	{0x0F12, 0x0010},

	{0x0028, 0xD000},
	{0x002A, 0x1082},
	{0x0F12, 0x03FF},

	{0x0028, 0xD000},
	{0x002A, 0x1084},
	{0x0F12, 0x03FF},

    {SEQUENCE_WAIT_MS, 0x0064},
    {SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_init_352x288[] =
{
	{0x0028, 0xD000},
	{0x002A, 0x1030},
	{0x0F12, 0x0000},
	{0x002A, 0x0014},
	{0x0F12, 0x0001},

	{SEQUENCE_WAIT_MS , 0x0014},// Delay 100 -> 20

	{0x0028, 0x7000},
	{0x002A, 0x2080},
	{0x0F12, 0xB510},	// 70002080               0
	{0x0F12, 0xF000},	// 70002082               2
	{0x0F12, 0xF8FC},	// 70002084               4
	{0x0F12, 0xBC10},	// 70002086               6
	{0x0F12, 0xBC08},	// 70002088               8
	{0x0F12, 0x4718},	// 7000208A               A
	{0x0F12, 0xB5FE},	// 7000208C               C
	{0x0F12, 0x0007},	// 7000208E               E
	{0x0F12, 0x683C},	// 70002090               0
	{0x0F12, 0x687E},	// 70002092               2
	{0x0F12, 0x1DA5},	// 70002094               4
	{0x0F12, 0x88A0},	// 70002096               6
	{0x0F12, 0x2800},	// 70002098               8
	{0x0F12, 0xD00B},	// 7000209A               A
	{0x0F12, 0x88A8},	// 7000209C               C
	{0x0F12, 0x2800},	// 7000209E               E
	{0x0F12, 0xD008},	// 700020A0               0
	{0x0F12, 0x8820},	// 700020A2               2
	{0x0F12, 0x8829},	// 700020A4               4
	{0x0F12, 0x4288},	// 700020A6               6
	{0x0F12, 0xD301},	// 700020A8               8
	{0x0F12, 0x1A40},	// 700020AA               A
	{0x0F12, 0xE000},	// 700020AC               C
	{0x0F12, 0x1A08},	// 700020AE               E
	{0x0F12, 0x9001},	// 700020B0               0
	{0x0F12, 0xE001},	// 700020B2               2
	{0x0F12, 0x2019},	// 700020B4               4
	{0x0F12, 0x9001},	// 700020B6               6
	{0x0F12, 0x497B},	// 700020B8               8
	{0x0F12, 0x466B},	// 700020BA               A
	{0x0F12, 0x8A48},	// 700020BC               C
	{0x0F12, 0x8118},	// 700020BE               E
	{0x0F12, 0x8A88},	// 700020C0               0
	{0x0F12, 0x8158},	// 700020C2               2
	{0x0F12, 0x4879},	// 700020C4               4
	{0x0F12, 0x8940},	// 700020C6               6
	{0x0F12, 0x0040},	// 700020C8               8
	{0x0F12, 0x2103},	// 700020CA               A
	{0x0F12, 0xF000},	// 700020CC               C
	{0x0F12, 0xF902},	// 700020CE               E
	{0x0F12, 0x88A1},	// 700020D0               0
	{0x0F12, 0x4288},	// 700020D2               2
	{0x0F12, 0xD908},	// 700020D4               4
	{0x0F12, 0x8828},	// 700020D6               6
	{0x0F12, 0x8030},	// 700020D8               8
	{0x0F12, 0x8868},	// 700020DA               A
	{0x0F12, 0x8070},	// 700020DC               C
	{0x0F12, 0x88A8},	// 700020DE               E
	{0x0F12, 0x6038},	// 700020E0               0
	{0x0F12, 0xBCFE},	// 700020E2               2
	{0x0F12, 0xBC08},	// 700020E4               4
	{0x0F12, 0x4718},	// 700020E6               6
	{0x0F12, 0x88A9},	// 700020E8               8
	{0x0F12, 0x4288},	// 700020EA               A
	{0x0F12, 0xD906},	// 700020EC               C
	{0x0F12, 0x8820},	// 700020EE               E
	{0x0F12, 0x8030},	// 700020F0               0
	{0x0F12, 0x8860},	// 700020F2               2
	{0x0F12, 0x8070},	// 700020F4               4
	{0x0F12, 0x88A0},	// 700020F6               6
	{0x0F12, 0x6038},	// 700020F8               8
	{0x0F12, 0xE7F2},	// 700020FA               A
	{0x0F12, 0x9801},	// 700020FC               C
	{0x0F12, 0xA902},	// 700020FE               E
	{0x0F12, 0xF000},	// 70002100               0
	{0x0F12, 0xF8EE},	// 70002102               2
	{0x0F12, 0x0033},	// 70002104               4
	{0x0F12, 0x0029},	// 70002106               6
	{0x0F12, 0x9A02},	// 70002108               8
	{0x0F12, 0x0020},	// 7000210A               A
	{0x0F12, 0xF000},	// 7000210C               C
	{0x0F12, 0xF8F0},	// 7000210E               E
	{0x0F12, 0x6038},	// 70002110               0
	{0x0F12, 0xE7E6},	// 70002112               2
	{0x0F12, 0xB5F0},	// 70002114               4
	{0x0F12, 0xB08B},	// 70002116               6
	{0x0F12, 0x0006},	// 70002118               8
	{0x0F12, 0x2000},	// 7000211A               A
	{0x0F12, 0x9004},	// 7000211C               C
	{0x0F12, 0x6835},	// 7000211E               E
	{0x0F12, 0x6874},	// 70002120               0
	{0x0F12, 0x68B0},	// 70002122               2
	{0x0F12, 0x900A},	// 70002124               4
	{0x0F12, 0x68F0},	// 70002126               6
	{0x0F12, 0x9009},	// 70002128               8
	{0x0F12, 0x4F60},	// 7000212A               A
	{0x0F12, 0x8979},	// 7000212C               C
	{0x0F12, 0x084A},	// 7000212E               E
	{0x0F12, 0x88A8},	// 70002130               0
	{0x0F12, 0x88A3},	// 70002132               2
	{0x0F12, 0x4298},	// 70002134               4
	{0x0F12, 0xD300},	// 70002136               6
	{0x0F12, 0x0018},	// 70002138               8
	{0x0F12, 0xF000},	// 7000213A               A
	{0x0F12, 0xF8E1},	// 7000213C               C
	{0x0F12, 0x9007},	// 7000213E               E
	{0x0F12, 0x0021},	// 70002140               0
	{0x0F12, 0x0028},	// 70002142               2
	{0x0F12, 0xAA04},	// 70002144               4
	{0x0F12, 0xF000},	// 70002146               6
	{0x0F12, 0xF8E3},	// 70002148               8
	{0x0F12, 0x9006},	// 7000214A               A
	{0x0F12, 0x88A8},	// 7000214C               C
	{0x0F12, 0x2800},	// 7000214E               E
	{0x0F12, 0xD102},	// 70002150               0
	{0x0F12, 0x27FF},	// 70002152               2
	{0x0F12, 0x1C7F},	// 70002154               4
	{0x0F12, 0xE047},	// 70002156               6
	{0x0F12, 0x88A0},	// 70002158               8
	{0x0F12, 0x2800},	// 7000215A               A
	{0x0F12, 0xD101},	// 7000215C               C
	{0x0F12, 0x2700},	// 7000215E               E
	{0x0F12, 0xE042},	// 70002160               0
	{0x0F12, 0x8820},	// 70002162               2
	{0x0F12, 0x466B},	// 70002164               4
	{0x0F12, 0x8198},	// 70002166               6
	{0x0F12, 0x8860},	// 70002168               8
	{0x0F12, 0x81D8},	// 7000216A               A
	{0x0F12, 0x8828},	// 7000216C               C
	{0x0F12, 0x8118},	// 7000216E               E
	{0x0F12, 0x8868},	// 70002170               0
	{0x0F12, 0x8158},	// 70002172               2
	{0x0F12, 0xA802},	// 70002174               4
	{0x0F12, 0xC803},	// 70002176               6
	{0x0F12, 0xF000},	// 70002178               8
	{0x0F12, 0xF8D2},	// 7000217A               A
	{0x0F12, 0x9008},	// 7000217C               C
	{0x0F12, 0x8ABA},	// 7000217E               E
	{0x0F12, 0x9808},	// 70002180               0
	{0x0F12, 0x466B},	// 70002182               2
	{0x0F12, 0x4342},	// 70002184               4
	{0x0F12, 0x9202},	// 70002186               6
	{0x0F12, 0x8820},	// 70002188               8
	{0x0F12, 0x8198},	// 7000218A               A
	{0x0F12, 0x8860},	// 7000218C               C
	{0x0F12, 0x81D8},	// 7000218E               E
	{0x0F12, 0x980A},	// 70002190               0
	{0x0F12, 0x9903},	// 70002192               2
	{0x0F12, 0xF000},	// 70002194               4
	{0x0F12, 0xF8C4},	// 70002196               6
	{0x0F12, 0x9A02},	// 70002198               8
	{0x0F12, 0x17D1},	// 7000219A               A
	{0x0F12, 0x0E09},	// 7000219C               C
	{0x0F12, 0x1889},	// 7000219E               E
	{0x0F12, 0x1209},	// 700021A0               0
	{0x0F12, 0x4288},	// 700021A2               2
	{0x0F12, 0xDD1F},	// 700021A4               4
	{0x0F12, 0x8820},	// 700021A6               6
	{0x0F12, 0x466B},	// 700021A8               8
	{0x0F12, 0x8198},	// 700021AA               A
	{0x0F12, 0x8860},	// 700021AC               C
	{0x0F12, 0x81D8},	// 700021AE               E
	{0x0F12, 0x980A},	// 700021B0               0
	{0x0F12, 0x9903},	// 700021B2               2
	{0x0F12, 0xF000},	// 700021B4               4
	{0x0F12, 0xF8B4},	// 700021B6               6
	{0x0F12, 0x9001},	// 700021B8               8
	{0x0F12, 0x8828},	// 700021BA               A
	{0x0F12, 0x466B},	// 700021BC               C
	{0x0F12, 0x8118},	// 700021BE               E
	{0x0F12, 0x8868},	// 700021C0               0
	{0x0F12, 0x8158},	// 700021C2               2
	{0x0F12, 0x980A},	// 700021C4               4
	{0x0F12, 0x9902},	// 700021C6               6
	{0x0F12, 0xF000},	// 700021C8               8
	{0x0F12, 0xF8AA},	// 700021CA               A
	{0x0F12, 0x8AB9},	// 700021CC               C
	{0x0F12, 0x9A08},	// 700021CE               E
	{0x0F12, 0x4351},	// 700021D0               0
	{0x0F12, 0x17CA},	// 700021D2               2
	{0x0F12, 0x0E12},	// 700021D4               4
	{0x0F12, 0x1851},	// 700021D6               6
	{0x0F12, 0x120A},	// 700021D8               8
	{0x0F12, 0x9901},	// 700021DA               A
	{0x0F12, 0xF000},	// 700021DC               C
	{0x0F12, 0xF890},	// 700021DE               E
	{0x0F12, 0x0407},	// 700021E0               0
	{0x0F12, 0x0C3F},	// 700021E2               2
	{0x0F12, 0xE000},	// 700021E4               4
	{0x0F12, 0x2700},	// 700021E6               6
	{0x0F12, 0x8820},	// 700021E8               8
	{0x0F12, 0x466B},	// 700021EA               A
	{0x0F12, 0xAA05},	// 700021EC               C
	{0x0F12, 0x8198},	// 700021EE               E
	{0x0F12, 0x8860},	// 700021F0               0
	{0x0F12, 0x81D8},	// 700021F2               2
	{0x0F12, 0x8828},	// 700021F4               4
	{0x0F12, 0x8118},	// 700021F6               6
	{0x0F12, 0x8868},	// 700021F8               8
	{0x0F12, 0x8158},	// 700021FA               A
	{0x0F12, 0xA802},	// 700021FC               C
	{0x0F12, 0xC803},	// 700021FE               E
	{0x0F12, 0x003B},	// 70002200               0
	{0x0F12, 0xF000},	// 70002202               2
	{0x0F12, 0xF895},	// 70002204               4
	{0x0F12, 0x88A1},	// 70002206               6
	{0x0F12, 0x88A8},	// 70002208               8
	{0x0F12, 0x003A},	// 7000220A               A
	{0x0F12, 0xF000},	// 7000220C               C
	{0x0F12, 0xF898},	// 7000220E               E
	{0x0F12, 0x0004},	// 70002210               0
	{0x0F12, 0xA804},	// 70002212               2
	{0x0F12, 0xC803},	// 70002214               4
	{0x0F12, 0x9A09},	// 70002216               6
	{0x0F12, 0x9B07},	// 70002218               8
	{0x0F12, 0xF000},	// 7000221A               A
	{0x0F12, 0xF889},	// 7000221C               C
	{0x0F12, 0xA806},	// 7000221E               E
	{0x0F12, 0xC805},	// 70002220               0
	{0x0F12, 0x0021},	// 70002222               2
	{0x0F12, 0xF000},	// 70002224               4
	{0x0F12, 0xF88C},	// 70002226               6
	{0x0F12, 0x6030},	// 70002228               8
	{0x0F12, 0xB00B},	// 7000222A               A
	{0x0F12, 0xBCF0},	// 7000222C               C
	{0x0F12, 0xBC08},	// 7000222E               E
	{0x0F12, 0x4718},	// 70002230               0
	{0x0F12, 0xB510},	// 70002232               2
	{0x0F12, 0x6800},	// 70002234               4
	{0x0F12, 0x2805},	// 70002236               6
	{0x0F12, 0xD11E},	// 70002238               8
	{0x0F12, 0x481D},	// 7000223A               A
	{0x0F12, 0x491D},	// 7000223C               C
	{0x0F12, 0x8800},	// 7000223E               E
	{0x0F12, 0x8348},	// 70002240               0
	{0x0F12, 0x2301},	// 70002242               2
	{0x0F12, 0x31A0},	// 70002244               4
	{0x0F12, 0x720B},	// 70002246               6
	{0x0F12, 0x2000},	// 70002248               8
	{0x0F12, 0x000A},	// 7000224A               A
	{0x0F12, 0x3A60},	// 7000224C               C
	{0x0F12, 0x7150},	// 7000224E               E
	{0x0F12, 0x71CB},	// 70002250               0
	{0x0F12, 0x22FF},	// 70002252               2
	{0x0F12, 0x3980},	// 70002254               4
	{0x0F12, 0x75CA},	// 70002256               6
	{0x0F12, 0x764A},	// 70002258               8
	{0x0F12, 0x000A},	// 7000225A               A
	{0x0F12, 0x3260},	// 7000225C               C
	{0x0F12, 0x7210},	// 7000225E               E
	{0x0F12, 0x7250},	// 70002260               0
	{0x0F12, 0x2008},	// 70002262               2
	{0x0F12, 0x7488},	// 70002264               4
	{0x0F12, 0x74C8},	// 70002266               6
	{0x0F12, 0x4813},	// 70002268               8
	{0x0F12, 0x217E},	// 7000226A               A
	{0x0F12, 0x43C9},	// 7000226C               C
	{0x0F12, 0x8101},	// 7000226E               E
	{0x0F12, 0x2306},	// 70002270               0
	{0x0F12, 0x5EC0},	// 70002272               2
	{0x0F12, 0xF000},	// 70002274               4
	{0x0F12, 0xF86C},	// 70002276               6
	{0x0F12, 0xBC10},	// 70002278               8
	{0x0F12, 0xBC08},	// 7000227A               A
	{0x0F12, 0x4718},	// 7000227C               C
	{0x0F12, 0xB570},	// 7000227E               E
	{0x0F12, 0x2200},	// 70002280               0
	{0x0F12, 0x490E},	// 70002282               2
	{0x0F12, 0x480E},	// 70002284               4
	{0x0F12, 0x2401},	// 70002286               6
	{0x0F12, 0xF000},	// 70002288               8
	{0x0F12, 0xF86A},	// 7000228A               A
	{0x0F12, 0x0022},	// 7000228C               C
	{0x0F12, 0x490D},	// 7000228E               E
	{0x0F12, 0x480D},	// 70002290               0
	{0x0F12, 0x2502},	// 70002292               2
	{0x0F12, 0xF000},	// 70002294               4
	{0x0F12, 0xF864},	// 70002296               6
	{0x0F12, 0x490C},	// 70002298               8
	{0x0F12, 0x480D},	// 7000229A               A
	{0x0F12, 0x002A},	// 7000229C               C
	{0x0F12, 0xF000},	// 7000229E               E
	{0x0F12, 0xF85F},	// 700022A0               0
	{0x0F12, 0xBC70},	// 700022A2               2
	{0x0F12, 0xBC08},	// 700022A4               4
	{0x0F12, 0x4718},	// 700022A6               6
	{0x0F12, 0x1A28},	// 700022A8               8
	{0x0F12, 0x7000},	// 700022AA               A
	{0x0F12, 0x0D64},	// 700022AC               C
	{0x0F12, 0x7000},	// 700022AE               E
	{0x0F12, 0x2370},	// 700022B0               0
	{0x0F12, 0x7000},	// 700022B2               2
	{0x0F12, 0x1C14},	// 700022B4               4
	{0x0F12, 0x7000},	// 700022B6               6
	{0x0F12, 0x14FA},	// 700022B8               8
	{0x0F12, 0x7000},	// 700022BA               A
	{0x0F12, 0x2115},	// 700022BC               C
	{0x0F12, 0x7000},	// 700022BE               E
	{0x0F12, 0x622F},	// 700022C0               0
	{0x0F12, 0x0000},	// 700022C2               2
	{0x0F12, 0x208D},	// 700022C4               4
	{0x0F12, 0x7000},	// 700022C6               6
	{0x0F12, 0x6445},	// 700022C8               8
	{0x0F12, 0x0000},	// 700022CA               A
	{0x0F12, 0x2233},	// 700022CC               C
	{0x0F12, 0x7000},	// 700022CE               E
	{0x0F12, 0x3B5B},	// 700022D0               0
	{0x0F12, 0x0000},	// 700022D2               2
	{0x0F12, 0x4778},	// 700022D4               4
	{0x0F12, 0x46C0},	// 700022D6               6
	{0x0F12, 0xF004},	// 700022D8               8
	{0x0F12, 0xE51F},	// 700022DA               A
	{0x0F12, 0xA464},	// 700022DC               C
	{0x0F12, 0x0000},	// 700022DE               E
	{0x0F12, 0x4778},	// 700022E0               0
	{0x0F12, 0x46C0},	// 700022E2               2
	{0x0F12, 0xC000},	// 700022E4               4
	{0x0F12, 0xE59F},	// 700022E6               6
	{0x0F12, 0xFF1C},	// 700022E8               8
	{0x0F12, 0xE12F},	// 700022EA               A
	{0x0F12, 0x6009},	// 700022EC               C
	{0x0F12, 0x0000},	// 700022EE               E
	{0x0F12, 0x4778},	// 700022F0               0
	{0x0F12, 0x46C0},	// 700022F2               2
	{0x0F12, 0xC000},	// 700022F4               4
	{0x0F12, 0xE59F},	// 700022F6               6
	{0x0F12, 0xFF1C},	// 700022F8               8
	{0x0F12, 0xE12F},	// 700022FA               A
	{0x0F12, 0x622F},	// 700022FC               C
	{0x0F12, 0x0000},	// 700022FE               E
	{0x0F12, 0x4778},	// 70002300               0
	{0x0F12, 0x46C0},	// 70002302               2
	{0x0F12, 0xC000},	// 70002304               4
	{0x0F12, 0xE59F},	// 70002306               6
	{0x0F12, 0xFF1C},	// 70002308               8
	{0x0F12, 0xE12F},	// 7000230A               A
	{0x0F12, 0x5F49},	// 7000230C               C
	{0x0F12, 0x0000},	// 7000230E               E
	{0x0F12, 0x4778},	// 70002310               0
	{0x0F12, 0x46C0},	// 70002312               2
	{0x0F12, 0xC000},	// 70002314               4
	{0x0F12, 0xE59F},	// 70002316               6
	{0x0F12, 0xFF1C},	// 70002318               8
	{0x0F12, 0xE12F},	// 7000231A               A
	{0x0F12, 0x5FC7},	// 7000231C               C
	{0x0F12, 0x0000},	// 7000231E               E
	{0x0F12, 0x4778},	// 70002320               0
	{0x0F12, 0x46C0},	// 70002322               2
	{0x0F12, 0xC000},	// 70002324               4
	{0x0F12, 0xE59F},	// 70002326               6
	{0x0F12, 0xFF1C},	// 70002328               8
	{0x0F12, 0xE12F},	// 7000232A               A
	{0x0F12, 0x5457},	// 7000232C               C
	{0x0F12, 0x0000},	// 7000232E               E
	{0x0F12, 0x4778},	// 70002330               0
	{0x0F12, 0x46C0},	// 70002332               2
	{0x0F12, 0xC000},	// 70002334               4
	{0x0F12, 0xE59F},	// 70002336               6
	{0x0F12, 0xFF1C},	// 70002338               8
	{0x0F12, 0xE12F},	// 7000233A               A
	{0x0F12, 0x5FA3},	// 7000233C               C
	{0x0F12, 0x0000},	// 7000233E               E
	{0x0F12, 0x4778},	// 70002340               0
	{0x0F12, 0x46C0},	// 70002342               2
	{0x0F12, 0xC000},	// 70002344               4
	{0x0F12, 0xE59F},	// 70002346               6
	{0x0F12, 0xFF1C},	// 70002348               8
	{0x0F12, 0xE12F},	// 7000234A               A
	{0x0F12, 0x51F9},	// 7000234C               C
	{0x0F12, 0x0000},	// 7000234E               E
	{0x0F12, 0x4778},	// 70002350               0
	{0x0F12, 0x46C0},	// 70002352               2
	{0x0F12, 0xC000},	// 70002354               4
	{0x0F12, 0xE59F},	// 70002356               6
	{0x0F12, 0xFF1C},	// 70002358               8
	{0x0F12, 0xE12F},	// 7000235A               A
	{0x0F12, 0x7633},	// 7000235C               C
	{0x0F12, 0x0000},	// 7000235E               E
	{0x0F12, 0x4778},	// 70002360               0
	{0x0F12, 0x46C0},	// 70002362               2
	{0x0F12, 0xC000},	// 70002364               4
	{0x0F12, 0xE59F},	// 70002366               6
	{0x0F12, 0xFF1C},	// 70002368               8
	{0x0F12, 0xE12F},	// 7000236A               A
	{0x0F12, 0xA007},	// 7000236C               C
	{0x0F12, 0x0000},	// 7000236E               E
	{0x0F12, 0x0360},	// 70002370               0
	{0x0F12, 0x6F4E},	// 70002372               2
	{0x0F12, 0x2076},	// 70002374               4
	{0x0F12, 0x3831},	// 70002376               6
	{0x0F12, 0x3220},	// 70002378               8
	{0x0F12, 0x3030},	// 7000237A               A
	{0x0F12, 0x0038},	// 7000237C               C
	{0x0F12, 0xE010},	// 7000237E               E
	{0x0F12, 0x0208},	// 70002380               0
	{0x0F12, 0x0038},	// 70002382               2
	// End of Trap and Patch (Last : 70002382h) // Total Size 772 (0x0304)
	{0x0028, 0xD000},
	{0x002A, 0x1000},
	{0x0F12, 0x0001},

	{0x0028, 0x7000},
	{0x002A, 0x0ADC},
	{0x0F12, 0x0AF0},	//#setot_uOnlineClocksDiv4OnlineC0             //2800
	{0x002A, 0x0AE2},
	{0x0F12, 0x222E},	//#setot_usSetRomWaitStatesSetRomThreshold4KHz //ROM Wait Threshold to 35MHz
	{0x002A, 0x0B94},
	{0x0F12, 0x0500},	//#awbb_GainsInit_0_:R    insInit
	{0x0F12, 0x0400},	//#awbb_GainsInit_1_:G    insInit
	{0x0F12, 0x0700},	//#awbb_GainsInit_2_:B    insInit
	{0x002A, 0x04A0},
	{0x0F12, 0x8000},	//#lt_uLeiInit:AE start   Init:AE
	{0x002A, 0x049A},
	{0x0F12, 0x003B},	//#lt_uMinExp             Exp



	//=================================================================================================
	//	Set CIS/APS/Analog
	//=================================================================================================
	// This registers are for FACTORY ONLY. If you change it without prior notification,
	// YOU are RESPONSIBLE for the FAILURE that will happen in the future.
	//=================================================================================================
	{0x0028, 0xD000},
	{0x002A, 0xF106},
	{0x0F12, 0x0001},	// L-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xF206},
	{0x0F12, 0x0001},	// F-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xC202},
	{0x0F12, 0x0700},	// tgr_coarse_integration_time[15:0]: revised by Ana 080115
	{0x002A, 0xF414},
	{0x0F12, 0x0030},	// aig_adc_sat[7:0] (aig_adc_sat[7:4]+1)*33mV + 690mV
	{0x002A, 0xC204},
	{0x0F12, 0x0100},	// tgr_analogue_code_global[12:0] Analog gain X8
	{0x002A, 0xF402},
	{0x0F12, 0x0092},	// aig_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_offset2[7:0]: revised by Ana 080425
	{0x002A, 0xF700},
	{0x0F12, 0x0040},	// bpradlc_control[7:0]: revised by Ana 080529 // bpradlc_nactive_pedestal[7:5],bpradlc_passthrough[1],bpradlc_bypass[0]
	{0x002A, 0xF708},
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_r[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gr[7:0]: revised by Tg 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gb[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_b[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_total[7:0]: revised by TG 080529
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_r[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gr[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gb[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_b[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_total[7:0]: revised by Ana 080425
	{0x0F12, 0x0001},	// bpradlc_adlc_en[7:0]: revised by TG 080529	// bpradlc_f_l_adlc_en[2],bpradlc_f_adlc_en[1],bpradlc_h_adlc_en[0]
	{0x0F12, 0x0015},	// bpradlc_adlc_option[7:0]: revised by TG 080425 // bpradlc_h_adlc_ch_sel[4], // bpradlc_max_data_clip_sel[3:2],bpradlc_f_adlc_max_data_clip_sel[1:0]
	{0x0F12, 0x0001},	// bpradlc_adlc_bpr_en[7:0] adlc_bpr_enable: revised by Ana 080112
	{0x0F12, 0x0040},	// bpradlc_adlc_bpr_thresh[7:0]: 080425
	{0x002A, 0xF48A},
	{0x0F12, 0x0048},	// aig_ld_ptr7[9:0] line OB start addr(04h~48h): revised by Ana 080911
	{0x002A, 0xF10A},
	{0x0F12, 0x008B},	// tgr_h_desc0_addr_end[10:0] line OB end addr(47h~8Bh): revised by Ana 080911 // line OB start - line OB end = 67d
	{0x002A, 0xF900},
	{0x0F12, 0x0067},	// cfpn_config: revised by Ana 080425 // cfpn_ref_gain[9:7],cpfn_ref_lines[6:5],cfpn_hbinning[4],cfpn_output_direct[3],// cfpn_overflow_protect[2],cpfn_passthrough[1],cfpn_bypass[0]
	{0x002A, 0xF406},
	{0x0F12, 0x0092},	// aig_cfpn_ref_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_cfpn_ref_offset2[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_cfpn_ref_gain[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_ld_ctrl[1:0] aig_ldb_en[1], aig_ld_en[0]
	{0x0F12, 0x0000},	// aig_row_id_ctrl[1:0]
	{0x002A, 0xF442},
	{0x0F12, 0x0000},	// aig_vavg[0]
	{0x0F12, 0x0000},	// aig_havg[0]=1b @H-avg mode: revised by Ana 080116
	{0x002A, 0xF448},
	{0x0F12, 0x0000},	// aig_sl_off[0]
	{0x002A, 0xF454},
	{0x0F12, 0x0001},	//Power save enable
	{0x0F12, 0x0001},	// aig_blst_en[0]
	{0x0F12, 0x0010},	// aig_blst_en_cintr[15:0]
	{0x0F12, 0x0000},	// aig_dshut_en[0]
	{0x002A, 0xF41A},
	{0x0F12, 0x00FF},	// aig_comp_bias[7:0] aig_comp2_bias[7:4], aig_comp1_bias[3:0]: revised by Ana 081013
	{0x0F12, 0x0003},	// aig_pix_bias[3:0]
	{0x002A, 0xF420},
	{0x0F12, 0x0030},	// aig_clp_lvl[7:0]: revised by Ana 080910(refer to 6AA)
	{0x002A, 0xF410},
	{0x0F12, 0x0001},	// aig_clp_sl_ctrl[0]
	{0x0F12, 0x0000},	// aig_cds_test[7:0], aig_cds_test[1]=1b @H-avg mode current save: revised by Ana 080116
	{0x002A, 0xF416},
	{0x0F12, 0x0001},	// aig_rmp_option[7:0]
	{0x002A, 0xF424},
	{0x0F12, 0x0000},	// aig_ref_option[7:0], aig_ref_option[0]=1b @H-avg mode odd cds off: revised by Ana 080116
	{0x002A, 0xF422},
	{0x0F12, 0x0000},	// aig_monit[7:0]
	{0x002A, 0xF41E},
	{0x0F12, 0x0000},	// aig_pd_pix[0]
	{0x002A, 0xF428},
	{0x0F12, 0x0000},	// aig_pd_cp[1:0] aig_pd_ncp[1], aig_pd_cp[0]
	{0x0F12, 0x0000},	// aig_pd_reg_pix[0]
	{0x0F12, 0x0000},	// aig_pd_reg_rg[0]
	{0x002A, 0xF430},
	{0x0F12, 0x0000},	// aig_pd_reg_tgsl[0]
	{0x0F12, 0x0000},	// aig_pd_reg_ntg[0]
	{0x0F12, 0x0008},	// aig_rosc_tune_cp[3:0]: revised by Ana 080418
	{0x0F12, 0x0005},	// aig_rosc_tune_ncp[3:0]
	{0x0F12, 0x000F},	// aig_cp_capa[3:0]
	{0x0F12, 0x0001},	// aig_reg_tune_pix[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_rg[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_tgsl[7:0]
	{0x0F12, 0x0010},	// aig_reg_tune_ntg[7:0] 0040 -> 0010
	{0x002A, 0xF4D6},
	{0x0F12, 0x00C0},	// aig_v15_tune[7:0]: revised by Ana 081010 // aig_v15_tune[7:4]=7h @sensor only Mode // aig_v15_tune[7:4]=9h @ISP 7.5fps Mode // aig_v15_tune[7:4]=Dh @ISP 15fps Mode
	{0x0F12, 0x0000},	//aig_vreg_sel[7:0] [9]h_test, [1]reg_sw_stby, [0]aig_reg_sel
	{0x002A, 0xF47C},
	{0x0F12, 0x000C},	//aig_ld_ptr0[4:0]
	{0x0F12, 0x0000},	//aig_ld_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF49A},
	{0x0F12, 0x0008},	//aig_sla_ptr0[3:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_sla_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4A2},
	{0x0F12, 0x0008},	//aig_slb_ptr0[7:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_slb_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4B2},
	{0x0F12, 0x0013},	//aig_rxa_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxa_ptr1[9:0]: revised by Ana 081010
	{0x0F12, 0x0013},	//aig_rxb_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxb_ptr1[9:0]: revised by Ana 081010
	{0x002A, 0xF4AA},
	{0x0F12, 0x009B},	//aig_txa_ptr0[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txa_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_txb_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txb_ptr1[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF474},
	{0x0F12, 0x0017},	//aig_s3_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x005F},	//aig_s3_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0017},	//aig_s4_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x008F},	//aig_s4_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF48C},
	{0x0F12, 0x0017},	//aig_clp_sl_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_clp_sl_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF4C8},
	{0x0F12, 0x0163},	//aig_off_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0193},	//aig_rmp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF490},
	{0x0F12, 0x0191},	//aig_comp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF418},
	{0x0F12, 0x0083},	//aig_dbs_option[7:0]: revised by Ana 081010

	//For ESD Check
	{0x0028, 0x7000},
	{0x002A, 0x0132},
	{0x0F12, 0xAAAA},	//#define REG_FWpid (For ESD Check)
	// * note : ESD Check Method.
	//	1. if (7000.0132 != AAAA ) reset // ( S/W reg. check)
	//	2. if (D000.1002 != 0000 ) reset // (internal Exception)

	//Set FPN Gain Input
	{0x002A, 0x1176},
	{0x0F12, 0x0020},	// fpn_GainInput[0]
	{0x0F12, 0x0040},	// fpn_GainInput[1]
	{0x0F12, 0x0080},	// fpn_GainInput[2]
	{0x0F12, 0x0100},	// fpn_GainInput[3]
	{0x0F12, 0x0014},	// fpn_GainOutput[0]
	{0x0F12, 0x000A},	// fpn_GainOutput[1]
	{0x0F12, 0x0008},	// fpn_GainOutput[2]
	{0x0F12, 0x0004},	// fpn_GainOutput[3]

	// Set CFPN Canceller
	{0x002A, 0x116E},
	{0x0F12, 0x0000},	// OvflProtect
	{0x0F12, 0x0000},	// bypassThrough
	{0x0F12, 0x0000},	// bypass
	{0x0F12, 0x0002},	// fpn.CollectMethod	// 0 : Center	1 : Right	2: LEFT
	{0x002A, 0x0AE8},
	{0x0F12, 0x0000},	// setot_bSendFpnToISP = ??

	// Set Forbidden Area
	{0x002A, 0x10EE},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_usRegCount // 0 value --> not use aig table 3
	{0x002A, 0x10F2},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_pSenModesRegsArray3[0]		//real register 0xD000F45A
	{0x002A, 0x1152},
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[48]    //real register 0xD000F4BA
	{0x0F12, 0x0028},	// senHal_SenRegsModes3_pSenModesRegsArray3[49]    //real register 0xD000F4BC
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[50]    //real register 0xD000F4BE
	{0x002A, 0x1148},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[43]    //real register 0xD000F4B0
	{0x002A, 0x1144},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[41]    //real register 0xD000F4AC
	{0x002A, 0x1150},
	{0x0F12, 0x0148},	// senHal_SenRegsModes3_pSenModesRegsArray3[47]    //real register 0xD000F4B8

	// Set AE Target
	{0x002A, 0x0F4C},
	{0x0F12, 0x003D},	// #TVAR_ae_BrAve



	//=================================================================================================
	//	Set Frame Rate
	//=================================================================================================
	{0x002A, 0x0484},
	{0x0F12, 0x61A8},	// #lt_uMaxExp1 	// 50ms
	{0x0F12, 0x0000},
	{0x0F12, 0xE848},	// #lt_uMaxExp2 	// 250ms
	{0x0F12, 0x0001},
	{0x0F12, 0x03C4},	// #lt_uCapMaxExp1	// 133ms
	{0x0F12, 0x0001},
	{0x0F12, 0xE848},	// #lt_uCapMaxExp2	// 250ms
	{0x0F12, 0x0001},
	{0x0F12, 0x04C0},	// #lt_uMaxAnGain1	// x4.75
	{0x0F12, 0x0700},	// #lt_uMaxAnGain2	// x7.0
	{0x0F12, 0x0100},	// #lt_uMaxDigGain
	{0x002A, 0x0F52},
	{0x0F12, 0x0007},	// #ae_StatMode



	//=================================================================================================
	//	Set Flicker  Current Flicker = 7000.164A //Mon_AFC_usFlExpQunat		0001:Off 1045:60Hz	1388:50Hz
	//=================================================================================================

	//1. Auto Flicker 60Hz Start
	{0x002A, 0x0B2E},
	{0x0F12, 0x0001},	// #AFC_Default60Hz 	1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	{0x002A, 0x03F8},
	{0x0F12, 0x007F},	// #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////2. Auto Flicker 50Hz Start
	//WRITE	70000B2E	0000	// #AFC_Default60Hz		1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	//WRITE 700003F8	007F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////3. Manual Flicker 60Hz


	//=================================================================================================
	//	Set PLL
	//=================================================================================================
	// Input CLK
	{0x002A, 0x01B8},
	{0x0F12, 0x5DC0},	//5DC0	//#REG_TC_IPRM_InClockLSBs	//24 -> 24.5M Hz(20081208)
	{0x0F12, 0x0000},	//#REG_TC_IPRM_InClockMSBs

	// Target Sys CLK
	{0x002A, 0x01CC},
	{0x0F12, 0x1770},	// #REG_TC_IPRM_OpClk4KHz_0 			//24Mhz

	// Target PVI CLK
	{0x0F12, 0x2318},	// #REG_TC_IPRM_MinOutRate4KHz_0	//36Mhz
	{0x0F12, 0x2338},	// #REG_TC_IPRM_MaxOutRate4KHz_0

	// PLL ETC
	{0x002A, 0x01C6},
	{0x0F12, 0x0003},	//#REG_TC_IPRM_UseNPviClocks
	{0x0F12, 0x0000},	//#REG_TC_IPRM_UseNMipiClocks
	{0x002A, 0x01C0},
	{0x0F12, 0x0000},	//#EG_TC_IPRM_ValidVActiveLow
	{0x002A, 0x01DE},
	{0x0F12, 0x0001},	//#REG_TC_IPRM_UseRegsAPI
	{0x0F12, 0x0001},	//#REG_TC_IPRM_InitParamsUpdated
	{0x002A, 0x1084},
	{0x0F12, 0x0001},	//#senHal_bUseAnalogBinning //Avg S.S
	{0x002A, 0x10B6},
	{0x0F12, 0x0048},	//#senHal_uMinColsAddAnalogBin



	//=================================================================================================
	//	Set Preview Config
	//=================================================================================================

	//* Preview Cropping Code *//
#ifdef VC_CAMERA_CROP
	{0x002A,	0x020A},
//	{0x0F12,	0x0640},	//#REG_TC_GP_PrevZoomReqInputWidth
//	{0x0F12,	0x0384},	//#REG_TC_GP_PrevZoomReqInputHeight
	{0x0F12,	0x0500},	//#REG_TC_GP_PrevZoomReqInputWidthOfs		0x500(1280)
	{0x0F12,	0x0418},	//#REG_TC_GP_PrevZoomReqInputHeightOfs		0x418(1048)

	//* Zoom Update *//
	{0x0F12,	0x0000},	//#REG_TC_GP_PrevZoomReqInputWidthOffset
	{0x0F12,	0x0098},	//#REG_TC_GP_PrevZoomReqInputHeightOffset

	{0x002A,	0x021A},
	{0x0F12,	0x0001},	// #REG_TC_GP_Inputs_ChangeRequest : 0x021A
#endif
	{0x002A, 0x0242},
	{0x0F12, 0x0160},  // #REG_0TC_PCFG_usWidth			// 0x160 => 352, 0x280 => 640,
	{0x0F12, 0x0120},  // #REG_0TC_PCFG_usHeight			// 0x120 => 288, 0x1E0 => 480,
	{0x0F12, 0x0005},	// #REG_0TC_PCFG_Format 						// YUV output
	{0x0F12, 0x2338},	// #REG_0TC_PCFG_usMaxOut4KHzRate
	{0x0F12, 0x2318},	// #REG_0TC_PCFG_usMinOut4KHzRate
//	{0x0F12, 0x446C},	// #REG_0TC_PCFG_usMaxOut4KHzRate
//	{0x0F12, 0x444C},	// #REG_0TC_PCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	// #REG_0TC_PCFG_PVIMask   (x2: PCLK rising x0: PCLK Falling)
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_uClockInd
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_usFrTimeType
//	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType		(0 : bin mode, 2 : No-bin mode)
	{0x0F12, 0x02AF},	//03E8},	// #REG_0TC_PCFG_usMaxFrTimeMsecMult10 // 10fps --> 15fps
	{0x0F12, 0x01F4},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 20fps
//	{0x0F12, 0x014D},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 30fps
	{0x002A, 0x0262},
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uPrevMirror -- X/Y flip 20090112
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uCaptureMirror -- X/Y flip 20090112
	//WRITE 70000262 000F  //#REG_0TC_PCFG_uPrevMirror
	//WRITE 70000264 000F  //#REG_0TC_PCFG_uCaptureMirror


	// Apply preview config
	{0x002A, 0x021C},
	{0x0F12, 0x0000},	//#REG_TC_GP_ActivePrevConfig	  //preview config0
	{0x002A, 0x0220},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevOpenAfterChange  //config 변경적용
	{0x002A, 0x01F8},
	{0x0F12, 0x0001},	//#REG_TC_GP_NewConfigSync
	{0x002A, 0x021E},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevConfigChanged



	//=================================================================================================
	//	Set Capture Config
	//=================================================================================================
	//WRITE 70000214	04C0	// #REG_TC_GP_CapZoomReqInputHeight	// Vsize = 1216
	//WRITE 70000216	0000	// #REG_TC_GP_CapZoomReqInputWidthOfs
	//WRITE 70000218	0000	// #REG_TC_GP_CapZoomReqInputHeightOfs
	//WRITE 7000021A	0001	// #REG_TC_GP_InputsChangeRequest
	//WRITE 700001F8	0001	// #REG_TC_GP_NewConfigSync
	//
#ifdef VC_CAMERA_CROP
	//* Capture Cropping Code *//
	{0x0028, 0x7000},
	{0x002A, 0x0212},
	{0x0F12, 0x0500}, //#REG_TC_GP_CapZoomReqInputWidth
	{0x0F12, 0x0418}, //#REG_TC_GP_CapZoomReqInputHeight
	{0x0F12, 0x0000}, //#REG_TC_GP_CapZoomReqInputWidthOfs
	{0x0F12, 0x0098}, //#REG_TC_GP_CapZoomReqInputHeightOfs
#endif
	{0x002A, 0x0302},
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uCaptureMode
	{0x0F12, 0x0500},	//#REG_0TC_CCFG_usWidth
	{0x0F12, 0x0418},	//#REG_0TC_CCFG_usHeight						// Vsize = 1216
	{0x0F12, 0x0005},	//#REG_0TC_CCFG_Format	   //5:YUV,9:JPEG
	{0x0F12, 0x2338},	//#REG_0TC_CCFG_usMaxOut4KHzRate
	{0x0F12, 0x2318},	//#REG_0TC_CCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	//#REG_0TC_CCFG_PVIMask (x2: PCLK rising	x0: PCLK Falling)
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uClockInd
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_usFrTimeType
	{0x0F12, 0x0002},	//#REG_0TC_CCFG_FrRateQualityType     (0 : bin mode, 2 : No-bin mode)
	{0x0F12, 0x09C4},	//07D0},	//#REG_0TC_CCFG_usMaxFrTimeMsecMult10 // 5fps --> 4fps
	{0x0F12, 0x0535},	//#REG_0TC_CCFG_usMinFrTimeMsecMult10 // 7.5fps

	//* Capture Configuration Update *//
	{0x002A, 0x01F8},
	{0x0F12, 0x0001}, //REG_TC_GP_NewConfigSync 0001 : update configuration
	{0x002a, 0x0226},
	{0x0f12, 0x0001}, //REG_TC_GP_CapConfigChanged 0001

	//=================================================================================================
	//	Set Periodic Mismatch (Use default value)
	//=================================================================================================



	//=================================================================================================
	//	Set AE Weights
	//=================================================================================================
	{0x002A, 0x0F5A},
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_0_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_1_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_2_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_3_
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_4_
	{0x0F12, 0x0101},	//0201	// #ae_WeightTbl_16_5_
	{0x0F12, 0x0100},	//0102	// #ae_WeightTbl_16_6_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_7_
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_8_
	{0x0F12, 0x0202},	//0202	// #ae_WeightTbl_16_9_
	{0x0F12, 0x0101},	//0202	// #ae_WeightTbl_16_10
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_11
	{0x0F12, 0x0201},	//0101	// #ae_WeightTbl_16_12
	{0x0F12, 0x0303},	//0402	// #ae_WeightTbl_16_13
	{0x0F12, 0x0102},	//0204	// #ae_WeightTbl_16_14
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_15
	{0x0F12, 0x0201},	//0201	// #ae_WeightTbl_16_16
	{0x0F12, 0x0505},	//0504	// #ae_WeightTbl_16_17
	{0x0F12, 0x0102},	//0405	// #ae_WeightTbl_16_18
	{0x0F12, 0x0000},	//0102	// #ae_WeightTbl_16_19
	{0x0F12, 0x0201},	//0501	// #ae_WeightTbl_16_20
	{0x0F12, 0x0303},	//0505	// #ae_WeightTbl_16_21
	{0x0F12, 0x0102},	//0505	// #ae_WeightTbl_16_22
	{0x0F12, 0x0000},	//0105	// #ae_WeightTbl_16_23
	{0x0F12, 0x0101},	//0503	// #ae_WeightTbl_16_24
	{0x0F12, 0x0202},	//0505	// #ae_WeightTbl_16_25
	{0x0F12, 0x0101},	//0505	// #ae_WeightTbl_16_26
	{0x0F12, 0x0000},	//0305	// #ae_WeightTbl_16_27
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_28
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_29
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_30
	{0x0F12, 0x0000},	//0303	// #ae_WeightTbl_16_31




	//=================================================================================================
	//	Set GAS & CCM White Point
	//=================================================================================================
	// param_start	TVAR_ash_AwbAshCord
	{0x002A, 0x0704},
	{0x0F12, 0x0090},	//000E	// #TVAR_ash_AwbAshCord_0_
	{0x0F12, 0x00B3},	//0012	// #TVAR_ash_AwbAshCord_1_
	{0x0F12, 0x00C3},	//0019	// #TVAR_ash_AwbAshCord_2_
	{0x0F12, 0x00E5},	//0034	// #TVAR_ash_AwbAshCord_3_
	{0x0F12, 0x0116},	//0044	// #TVAR_ash_AwbAshCord_4_
	{0x0F12, 0x0198},	//0052	// #TVAR_ash_AwbAshCord_5_
	{0x0F12, 0x0200},	//0058	// #TVAR_ash_AwbAshCord_6_

	// param_start	wbt_AwbCcmCord
	{0x002A, 0x06F2},
	{0x0F12, 0x00B3},	//0016	// #SARR_AwbCcmCord_0_	Hor
	{0x0F12, 0x00E5},	//001E	// #SARR_AwbCcmCord_1_	IncaA
	{0x0F12, 0x0100},	//0024	// #SARR_AwbCcmCord_2_	WW
	{0x0F12, 0x0116},	//0038	// #SARR_AwbCcmCord_3_	CW
	{0x0F12, 0x0150},	//004C	// #SARR_AwbCcmCord_4_	D50
	{0x0F12, 0x0198},	//0058	// #SARR_AwbCcmCord_5_	D65

	// Set Target Brightness Control
	{0x002A, 0x103E},
	{0x0F12, 0x0000},	// #SARR_IllumType_0_
	{0x0F12, 0x0009},	// #SARR_IllumType_1_
	{0x0F12, 0x0018},	// #SARR_IllumType_2_
	{0x0F12, 0x0032},	// #SARR_IllumType_3_
	{0x0F12, 0x004A},	// #SARR_IllumType_4_
	{0x0F12, 0x0051},	// #SARR_IllumType_5_
	{0x0F12, 0x0056},	// #SARR_IllumType_6_
	{0x0F12, 0x010C},	// #SARe_2_R_IllumTypeF_0_
	{0x0F12, 0x010C},	// #SARe_3_R_IllumTypeF_1_
	{0x0F12, 0x0109},	// #SARe_4_R_IllumTypeF_2_
	{0x0F12, 0x0105},	// #SARe_5_R_IllumTypeF_3_
	{0x0F12, 0x0102},	// #SARe_6_R_IllumTypeF_4_
	{0x0F12, 0x00FB},	// #SARR_IllumTypeF_5_
	{0x0F12, 0x00F8},	// #SARR_IllumTypeF_6_

	// Indoor GAS Alpha
	{0x002A, 0x0712},
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[0]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[1]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[2]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[3]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[4]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[5]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[6]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[7]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[8]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[9]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[10]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[11]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[12]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[13]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[14]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[15]
	{0x0F12, 0x00E0},	// #TVAR_ash_GASAlpha[16]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[17]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[18]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[19]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[20]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[21]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[22]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[23]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[24]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[25]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[26]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[27]

	// Outdoor GAS Alpha
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_0_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_1_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_2_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_3_

	// GAS LUT Start Address
	{0x002A, 0x0754},
	{0x0F12, 0x2388},	//#TVAR_ash_pGAS
	{0x0F12, 0x7000},	//#TVAR_ash_pGAS

	// GAS LUT
	//							//1030	//1027
	{0x002A, 0x2388},
	{0x0F12, 0x01C9},	//#TVAR_ash_pGAS[0]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[1]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[2]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[3]
	{0x0F12, 0x0105},	//#TVAR_ash_pGAS[4]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[5]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[6]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[7]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[8]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[9]
	{0x0F12, 0x0160},	//#TVAR_ash_pGAS[10]
	{0x0F12, 0x0189},	//#TVAR_ash_pGAS[11]
	{0x0F12, 0x01D0},	//#TVAR_ash_pGAS[12]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[13]
	{0x0F12, 0x016A},	//#TVAR_ash_pGAS[14]
	{0x0F12, 0x0126},	//#TVAR_ash_pGAS[15]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[16]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[17]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[18]
	{0x0F12, 0x009B},	//#TVAR_ash_pGAS[19]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[20]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[21]
	{0x0F12, 0x00EF},	//#TVAR_ash_pGAS[22]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[23]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[24]
	{0x0F12, 0x019C},	//#TVAR_ash_pGAS[25]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[26]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[27]
	{0x0F12, 0x00E2},	//#TVAR_ash_pGAS[28]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[29]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[30]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[31]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[32]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[33]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[34]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[35]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[36]
	{0x0F12, 0x0133},	//#TVAR_ash_pGAS[37]
	{0x0F12, 0x016E},	//#TVAR_ash_pGAS[38]
	{0x0F12, 0x0140},	//#TVAR_ash_pGAS[39]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[40]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[41]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[42]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[43]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[44]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[45]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[46]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[47]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[48]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[49]
	{0x0F12, 0x010D},	//#TVAR_ash_pGAS[50]
	{0x0F12, 0x0157},	//#TVAR_ash_pGAS[51]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[52]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[53]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[54]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[55]
	{0x0F12, 0x002F},	//#TVAR_ash_pGAS[56]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[57]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[58]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[59]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[60]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[61]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[62]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[63]
	{0x0F12, 0x0142},	//#TVAR_ash_pGAS[64]
	{0x0F12, 0x011A},	//#TVAR_ash_pGAS[65]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[66]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[67]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[68]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[69]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[70]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[71]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[72]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[73]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[74]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[75]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[76]
	{0x0F12, 0x013B},	//#TVAR_ash_pGAS[77]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[78]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[79]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[80]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[81]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[82]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[83]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[84]
	{0x0F12, 0x0014},	//#TVAR_ash_pGAS[85]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[86]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[87]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[88]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[89]
	{0x0F12, 0x013F},	//#TVAR_ash_pGAS[90]
	{0x0F12, 0x012C},	//#TVAR_ash_pGAS[91]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[92]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[93]
	{0x0F12, 0x0067},	//#TVAR_ash_pGAS[94]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[95]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[96]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[97]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[98]
	{0x0F12, 0x004B},	//#TVAR_ash_pGAS[99]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[100]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[101]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[102]
	{0x0F12, 0x0150},	//#TVAR_ash_pGAS[103]
	{0x0F12, 0x0149},	//#TVAR_ash_pGAS[104]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[105]
	{0x0F12, 0x00C5},	//#TVAR_ash_pGAS[106]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[107]
	{0x0F12, 0x0061},	//#TVAR_ash_pGAS[108]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[109]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[110]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[111]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[112]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[113]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[114]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[115]
	{0x0F12, 0x016D},	//#TVAR_ash_pGAS[116]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[117]
	{0x0F12, 0x013A},	//#TVAR_ash_pGAS[118]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[119]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[120]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[121]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[122]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[123]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[124]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[125]
	{0x0F12, 0x00D8},	//#TVAR_ash_pGAS[126]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[127]
	{0x0F12, 0x0154},	//#TVAR_ash_pGAS[128]
	{0x0F12, 0x0186},	//#TVAR_ash_pGAS[129]
	{0x0F12, 0x018F},	//#TVAR_ash_pGAS[130]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[131]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[132]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[133]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[134]
	{0x0F12, 0x00B4},	//#TVAR_ash_pGAS[135]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[136]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[137]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[138]
	{0x0F12, 0x0112},	//#TVAR_ash_pGAS[139]
	{0x0F12, 0x014F},	//#TVAR_ash_pGAS[140]
	{0x0F12, 0x0185},	//#TVAR_ash_pGAS[141]
	{0x0F12, 0x01A1},	//#TVAR_ash_pGAS[142]
	{0x0F12, 0x0168},	//#TVAR_ash_pGAS[143]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[144]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[145]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[146]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[147]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[148]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[149]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[150]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[151]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[152]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[153]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[154]
	{0x0F12, 0x0163},	//#TVAR_ash_pGAS[155]
	{0x0F12, 0x0131},	//#TVAR_ash_pGAS[156]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[157]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[158]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[159]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[160]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[161]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[162]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[163]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[164]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[165]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[166]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[167]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[168]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[169]
	{0x0F12, 0x00D2},	//#TVAR_ash_pGAS[170]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[171]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[172]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[173]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[174]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[175]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[176]
	{0x0F12, 0x0059},	//#TVAR_ash_pGAS[177]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[178]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[179]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[180]
	{0x0F12, 0x010C},	//#TVAR_ash_pGAS[181]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[182]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[183]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[184]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[185]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[186]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[187]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[188]
	{0x0F12, 0x0025},	//#TVAR_ash_pGAS[189]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[190]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[191]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[192]
	{0x0F12, 0x00C2},	//#TVAR_ash_pGAS[193]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[194]
	{0x0F12, 0x00DD},	//#TVAR_ash_pGAS[195]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[196]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[197]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[198]
	{0x0F12, 0x001E},	//#TVAR_ash_pGAS[199]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[200]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[201]
	{0x0F12, 0x0010},	//#TVAR_ash_pGAS[202]
	{0x0F12, 0x0026},	//#TVAR_ash_pGAS[203]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[204]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[205]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[206]
	{0x0F12, 0x00EC},	//#TVAR_ash_pGAS[207]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[208]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[209]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[210]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[211]
	{0x0F12, 0x0017},	//#TVAR_ash_pGAS[212]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[213]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[214]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[215]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[216]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[217]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[218]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[219]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[220]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[221]
	{0x0F12, 0x009A},	//#TVAR_ash_pGAS[222]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[223]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[224]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[225]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[226]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[227]
	{0x0F12, 0x000F},	//#TVAR_ash_pGAS[228]
	{0x0F12, 0x0027},	//#TVAR_ash_pGAS[229]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[230]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[231]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[232]
	{0x0F12, 0x00F1},	//#TVAR_ash_pGAS[233]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[234]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[235]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[236]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[237]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[238]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[239]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[240]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[241]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[242]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[243]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[244]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[245]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[246]
	{0x0F12, 0x00FA},	//#TVAR_ash_pGAS[247]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[248]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[249]
	{0x0F12, 0x0064},	//#TVAR_ash_pGAS[250]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[251]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[252]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[253]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[254]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[255]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[256]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[257]
	{0x0F12, 0x00DF},	//#TVAR_ash_pGAS[258]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[259]
	{0x0F12, 0x0110},	//#TVAR_ash_pGAS[260]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[261]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[262]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[263]
	{0x0F12, 0x006E},	//#TVAR_ash_pGAS[264]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[265]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[266]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[267]
	{0x0F12, 0x0083},	//#TVAR_ash_pGAS[268]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[269]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[270]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[271]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[272]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[273]
	{0x0F12, 0x0104},	//#TVAR_ash_pGAS[274]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[275]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[276]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[277]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[278]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[279]
	{0x0F12, 0x0098},	//#TVAR_ash_pGAS[280]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[281]
	{0x0F12, 0x00D1},	//#TVAR_ash_pGAS[282]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[283]
	{0x0F12, 0x0127},	//#TVAR_ash_pGAS[284]
	{0x0F12, 0x0146},	//#TVAR_ash_pGAS[285]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[286]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[287]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[288]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[289]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[290]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[291]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[292]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[293]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[294]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[295]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[296]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[297]
	{0x0F12, 0x0180},	//#TVAR_ash_pGAS[298]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[299]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[300]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[301]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[302]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[303]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[304]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[305]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[306]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[307]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[308]
	{0x0F12, 0x00EE},	//#TVAR_ash_pGAS[309]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[310]
	{0x0F12, 0x014C},	//#TVAR_ash_pGAS[311]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[312]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[313]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[314]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[315]
	{0x0F12, 0x0057},	//#TVAR_ash_pGAS[316]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[317]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[318]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[319]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[320]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[321]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[322]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[323]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[324]
	{0x0F12, 0x00F6},	//#TVAR_ash_pGAS[325]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[326]
	{0x0F12, 0x0081},	//#TVAR_ash_pGAS[327]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[328]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[329]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[330]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[331]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[332]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[333]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[334]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[335]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[336]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[337]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[338]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[339]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[340]
	{0x0F12, 0x0041},	//#TVAR_ash_pGAS[341]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[342]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[343]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[344]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[345]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[346]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[347]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[348]
	{0x0F12, 0x00BA},	//#TVAR_ash_pGAS[349]
	{0x0F12, 0x00F7},	//#TVAR_ash_pGAS[350]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[351]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[352]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[353]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[354]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[355]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[356]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[357]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[358]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[359]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[360]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[361]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[362]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[363]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[364]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[365]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[366]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[367]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[368]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[369]
	{0x0F12, 0x0002},	//#TVAR_ash_pGAS[370]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[371]
	{0x0F12, 0x0023},	//#TVAR_ash_pGAS[372]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[373]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[374]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[375]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[376]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[377]
	{0x0F12, 0x00B0},	//#TVAR_ash_pGAS[378]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[379]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[380]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[381]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[382]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[383]
	{0x0F12, 0x001C},	//#TVAR_ash_pGAS[384]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[385]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[386]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[387]
	{0x0F12, 0x00B7},	//#TVAR_ash_pGAS[388]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[389]
	{0x0F12, 0x0106},	//#TVAR_ash_pGAS[390]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[391]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[392]
	{0x0F12, 0x006C},	//#TVAR_ash_pGAS[393]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[394]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[395]
	{0x0F12, 0x0033},	//#TVAR_ash_pGAS[396]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[397]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[398]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[399]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[400]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[401]
	{0x0F12, 0x0101},	//#TVAR_ash_pGAS[402]
	{0x0F12, 0x0120},	//#TVAR_ash_pGAS[403]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[404]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[405]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[406]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[407]
	{0x0F12, 0x0062},	//#TVAR_ash_pGAS[408]
	{0x0F12, 0x005C},	//#TVAR_ash_pGAS[409]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[410]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[411]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[412]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[413]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[414]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[415]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[416]
	{0x0F12, 0x0115},	//#TVAR_ash_pGAS[417]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[418]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[419]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[420]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[421]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[422]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[423]
	{0x0F12, 0x00A4},	//#TVAR_ash_pGAS[424]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[425]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[426]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[427]
	{0x0F12, 0x0130},	//#TVAR_ash_pGAS[428]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[429]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[430]
	{0x0F12, 0x00C6},	//#TVAR_ash_pGAS[431]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[432]
	{0x0F12, 0x0090},	//#TVAR_ash_pGAS[433]
	{0x0F12, 0x0085},	//#TVAR_ash_pGAS[434]
	{0x0F12, 0x0082},	//#TVAR_ash_pGAS[435]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[436]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[437]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[438]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[439]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[440]
	{0x0F12, 0x0135},	//#TVAR_ash_pGAS[441]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[442]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[443]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[444]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[445]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[446]
	{0x0F12, 0x005D},	//#TVAR_ash_pGAS[447]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[448]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[449]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[450]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[451]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[452]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[453]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[454]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[455]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[456]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[457]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[458]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[459]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[460]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[461]
	{0x0F12, 0x003D},	//#TVAR_ash_pGAS[462]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[463]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[464]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[465]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[466]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[467]
	{0x0F12, 0x00B5},	//#TVAR_ash_pGAS[468]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[469]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[470]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[471]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[472]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[473]
	{0x0F12, 0x001A},	//#TVAR_ash_pGAS[474]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[475]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[476]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[477]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[478]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[479]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[480]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[481]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[482]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[483]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[484]
	{0x0F12, 0x0018},	//#TVAR_ash_pGAS[485]
	{0x0F12, 0x000A},	//#TVAR_ash_pGAS[486]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[487]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[488]
	{0x0F12, 0x001F},	//#TVAR_ash_pGAS[489]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[490]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[491]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[492]
	{0x0F12, 0x00B6},	//#TVAR_ash_pGAS[493]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[494]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[495]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[496]
	{0x0F12, 0x0028},	//#TVAR_ash_pGAS[497]
	{0x0F12, 0x0012},	//#TVAR_ash_pGAS[498]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[499]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[500]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[501]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[502]
	{0x0F12, 0x0030},	//#TVAR_ash_pGAS[503]
	{0x0F12, 0x0050},	//#TVAR_ash_pGAS[504]
	{0x0F12, 0x007B},	//#TVAR_ash_pGAS[505]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[506]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[507]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[508]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[509]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[510]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[511]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[512]
	{0x0F12, 0x0001},	//#TVAR_ash_pGAS[513]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[514]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[515]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[516]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[517]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[518]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[519]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[520]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[521]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[522]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[523]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[524]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[525]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[526]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[527]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[528]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[529]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[530]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[531]
	{0x0F12, 0x00AE},	//#TVAR_ash_pGAS[532]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[533]
	{0x0F12, 0x0097},	//#TVAR_ash_pGAS[534]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[535]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[536]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[537]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[538]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[539]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[540]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[541]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[542]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[543]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[544]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[545]
	{0x0F12, 0x00D3},	//#TVAR_ash_pGAS[546]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[547]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[548]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[549]
	{0x0F12, 0x0058},	//#TVAR_ash_pGAS[550]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[551]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[552]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[553]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[554]
	{0x0F12, 0x006D},	//#TVAR_ash_pGAS[555]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[556]
	{0x0F12, 0x00AC},	//#TVAR_ash_pGAS[557]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[558]
	{0x0F12, 0x00F5},	//#TVAR_ash_pGAS[559]
	{0x0F12, 0x00D4},	//#TVAR_ash_pGAS[560]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[561]
	{0x0F12, 0x008E},	//#TVAR_ash_pGAS[562]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[563]
	{0x0F12, 0x0070},	//#TVAR_ash_pGAS[564]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[565]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[566]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[567]
	{0x0F12, 0x0093},	//#TVAR_ash_pGAS[568]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[569]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[570]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[571]



	//=================================================================================================
	//	Set Gamma LUT
	//=================================================================================================
	//							//New	//Hong	//ORG
	{0x002A, 0x04CC},
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[0][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[0][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[0][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[0][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[0][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[0][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[0][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[0][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[0][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[0][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[0][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[0][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[0][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[0][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[0][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[0][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[1][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[1][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[1][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[1][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[1][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[1][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[1][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[1][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[1][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[1][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[1][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[1][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[1][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[1][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[1][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[1][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[2][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[2][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[2][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[2][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[2][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[2][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[2][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[2][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[2][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[2][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[2][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[2][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[2][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[2][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[2][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[2][15]



	//=================================================================================================
	//	Set AWB
	//=================================================================================================
	{0x002A, 0x0DA8},
	{0x0F12, 0x0020},	// #awbb_LowBr0_NBzone	// 2008.04.24. eliminate to the purple color at low light.
	{0x002A, 0x0E8C},
	{0x0F12, 0x0000},	// #awbb_LowBr0_PatchNumZone
	{0x002A, 0x0D6C},
	{0x0F12, 0x0040},	// #awbb_YMedMoveToYAv

	// Indoor Gray Zone
	{0x002A, 0x0B9C},
	{0x0F12, 0x0339},	//0392	// #awbb_IndoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x0393},	//03A4	// #awbb_IndoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x031B},	//036F	// #awbb_IndoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x039C},	//03AE	// #awbb_IndoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x030A},	//034E	// #awbb_IndoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0398},	//03A2	// #awbb_IndoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x02F3},	//0331	// #awbb_IndoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0392},	//0392	// #awbb_IndoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x02E0},	//0315	// #awbb_IndoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0388},	//0381	// #awbb_IndoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x02CB},	//02F8	// #awbb_IndoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0374},	//036C	// #awbb_IndoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x02BB},	//02E1	// #awbb_IndoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x034B},	//0358	// #awbb_IndoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x02AA},	//02C5	// #awbb_IndoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x0336},	//033B	// #awbb_IndoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x029C},	//02B1	// #awbb_IndoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0321},	//0323	// #awbb_IndoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x028B},	//029C	// #awbb_IndoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x030B},	//030F	// #awbb_IndoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x027B},	//028B	// #awbb_IndoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02FD},	//02FA	// #awbb_IndoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x0268},	//027C	// #awbb_IndoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x02EA},	//02E2	// #awbb_IndoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x0259},	//0268	// #awbb_IndoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x02D8},	//02D1	// #awbb_IndoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x024C},	//0259	// #awbb_IndoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x02CB},	//02BF	// #awbb_IndoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x023D},	//024C	// #awbb_IndoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x02BD},	//02B1	// #awbb_IndoorGrZones_m_BGrid_14__m_right
	{0x0F12, 0x0231},	//023D	// #awbb_IndoorGrZones_m_BGrid_15__m_left
	{0x0F12, 0x02B1},	//02A2	// #awbb_IndoorGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0222},	//0231	// #awbb_IndoorGrZones_m_BGrid_16__m_left
	{0x0F12, 0x02A8},	//0299	// #awbb_IndoorGrZones_m_BGrid_16__m_right
	{0x0F12, 0x0215},	//0222	// #awbb_IndoorGrZones_m_BGrid_17__m_left
	{0x0F12, 0x029B},	//0290	// #awbb_IndoorGrZones_m_BGrid_17__m_right
	{0x0F12, 0x0208},	//0215	// #awbb_IndoorGrZones_m_BGrid_18__m_left
	{0x0F12, 0x0290},	//0288	// #awbb_IndoorGrZones_m_BGrid_18__m_right
	{0x0F12, 0x01F7},	//0208	// #awbb_IndoorGrZones_m_BGrid_19__m_left
	{0x0F12, 0x0287},	//0281	// #awbb_IndoorGrZones_m_BGrid_19__m_right
	{0x0F12, 0x01E6},	//01F7	// #awbb_IndoorGrZones_m_BGrid_20__m_left
	{0x0F12, 0x027F},	//0279	// #awbb_IndoorGrZones_m_BGrid_20__m_right
	{0x0F12, 0x01D9},	//01E6	// #awbb_IndoorGrZones_m_BGrid_21__m_left
	{0x0F12, 0x0279},	//026E	// #awbb_IndoorGrZones_m_BGrid_21__m_right
	{0x0F12, 0x01CC},	//01D9	// #awbb_IndoorGrZones_m_BGrid_22__m_left
	{0x0F12, 0x0272},	//0268	// #awbb_IndoorGrZones_m_BGrid_22__m_right
	{0x0F12, 0x01C1},	//01CC	// #awbb_IndoorGrZones_m_BGrid_23__m_left
	{0x0F12, 0x026C},	//0263	// #awbb_IndoorGrZones_m_BGrid_23__m_right
	{0x0F12, 0x01B3},	//01C1	// #awbb_IndoorGrZones_m_BGrid_24__m_left
	{0x0F12, 0x0265},	//0260	// #awbb_IndoorGrZones_m_BGrid_24__m_right
	{0x0F12, 0x01AA},	//01B3	// #awbb_IndoorGrZones_m_BGrid_25__m_left
	{0x0F12, 0x025C},	//0259	// #awbb_IndoorGrZones_m_BGrid_25__m_right
	{0x0F12, 0x019F},	//01AA	// #awbb_IndoorGrZones_m_BGrid_26__m_left
	{0x0F12, 0x0250},	//0253	// #awbb_IndoorGrZones_m_BGrid_26__m_right
	{0x0F12, 0x019B},	//019F	// #awbb_IndoorGrZones_m_BGrid_27__m_left
	{0x0F12, 0x0247},	//0248	// #awbb_IndoorGrZones_m_BGrid_27__m_right
	{0x0F12, 0x0197},	//019B	// #awbb_IndoorGrZones_m_BGrid_28__m_left
	{0x0F12, 0x0239},	//023F	// #awbb_IndoorGrZones_m_BGrid_28__m_right
	{0x0F12, 0x019B},	//0197	// #awbb_IndoorGrZones_m_BGrid_29__m_left
	{0x0F12, 0x021D},	//0239	// #awbb_IndoorGrZones_m_BGrid_29__m_right
	{0x0F12, 0x019D},	//019B	// #awbb_IndoorGrZones_m_BGrid_30__m_left
	{0x0F12, 0x0202},	//021D	// #awbb_IndoorGrZones_m_BGrid_30__m_right
	{0x0F12, 0x01AC},	//019D	// #awbb_IndoorGrZones_m_BGrid_31__m_left
	{0x0F12, 0x01D3},	//020A	// #awbb_IndoorGrZones_m_BGrid_31__m_right
	{0x0F12, 0x0000},	//01AC	// #awbb_IndoorGrZones_m_BGrid_32__m_left
	{0x0F12, 0x0000},	//01D3	// #awbb_IndoorGrZones_m_BGrid_32__m_right
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_left
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_right

	{0x002A, 0x0C3C},
	{0x0F12, 0x0004},	// #awbb_IndoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0138},	// #awbb_IndoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x0F12, 0x0000},

	// Outdoor Gray Zone
	{0x0F12, 0x0255},	//#awbb_OutdoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x0236},	//#awbb_OutdoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x0290},	//#awbb_OutdoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x0227},	//#awbb_OutdoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0297},	//#awbb_OutdoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x0210},	//#awbb_OutdoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0293},	//#awbb_OutdoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0200},	//#awbb_OutdoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x028B},	//#awbb_OutdoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x01F1},	//#awbb_OutdoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0286},	//#awbb_OutdoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x0282},	//#awbb_OutdoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x01E0},	//#awbb_OutdoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x027B},	//#awbb_OutdoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x01DE},	//#awbb_OutdoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0274},	//#awbb_OutdoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01DF},	//#awbb_OutdoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x0263},	//#awbb_OutdoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01F6},	//#awbb_OutdoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x0253},	//#awbb_OutdoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x020B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x023B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_right

	{0x002A, 0x0CB8},
	{0x0F12, 0x0004},	// #awbb_OutdoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x000C},	//#awbb_OutdoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0217},	//#awbb_OutdoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high
	{0x0F12, 0x0000},

	// Low Brightness Gray Zone
	{0x0F12, 0x0324},	//031E  // #awbb_LowBrGrZones_m_BGrid_0__m_left
	{0x0F12, 0x03A1},	//037E  // #awbb_LowBrGrZones_m_BGrid_0__m_right
	{0x0F12, 0x02F2},	//02CF  // #awbb_LowBrGrZones_m_BGrid_1__m_left
	{0x0F12, 0x03BA},	//0389  // #awbb_LowBrGrZones_m_BGrid_1__m_right
	{0x0F12, 0x02C4},	//02A6  // #awbb_LowBrGrZones_m_BGrid_2__m_left
	{0x0F12, 0x03B6},	//0386  // #awbb_LowBrGrZones_m_BGrid_2__m_right
	{0x0F12, 0x029E},	//0282  // #awbb_LowBrGrZones_m_BGrid_3__m_left
	{0x0F12, 0x03A6},	//0374  // #awbb_LowBrGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0283},	//0259  // #awbb_LowBrGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0380},	//0352  // #awbb_LowBrGrZones_m_BGrid_4__m_right
	{0x0F12, 0x025F},	//0239  // #awbb_LowBrGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0344},	//0330  // #awbb_LowBrGrZones_m_BGrid_5__m_right
	{0x0F12, 0x0241},	//021E  // #awbb_LowBrGrZones_m_BGrid_6__m_left
	{0x0F12, 0x031F},	//031B  // #awbb_LowBrGrZones_m_BGrid_6__m_right
	{0x0F12, 0x0227},	//020B  // #awbb_LowBrGrZones_m_BGrid_7__m_left
	{0x0F12, 0x02FC},	//0300  // #awbb_LowBrGrZones_m_BGrid_7__m_right
	{0x0F12, 0x020A},	//01F1  // #awbb_LowBrGrZones_m_BGrid_8__m_left
	{0x0F12, 0x02D9},	//02DD  // #awbb_LowBrGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01F1},	//01E0  // #awbb_LowBrGrZones_m_BGrid_9__m_left
	{0x0F12, 0x02C4},	//02C4  // #awbb_LowBrGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01D0},	//01CD  // #awbb_LowBrGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02B2},	//02AF  // #awbb_LowBrGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01B8},	//019E  // #awbb_LowBrGrZones_m_BGrid_11__m_left
	{0x0F12, 0x029E},	//02A0  // #awbb_LowBrGrZones_m_BGrid_11__m_right
	{0x0F12, 0x019F},	//0187  // #awbb_LowBrGrZones_m_BGrid_12__m_left
	{0x0F12, 0x0290},	//028C  // #awbb_LowBrGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0189},	//017D  // #awbb_LowBrGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0282},	//027E  // #awbb_LowBrGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0182},	//0179  // #awbb_LowBrGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0271},	//0258  // #awbb_LowBrGrZones_m_BGrid_14__m_right
	{0x0F12, 0x017F},	//0186  // #awbb_LowBrGrZones_m_BGrid_15__m_left
	{0x0F12, 0x0254},	//0232  // #awbb_LowBrGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0183},	//01BD  // #awbb_LowBrGrZones_m_BGrid_16__m_left
	{0x0F12, 0x022D},	//01FB  // #awbb_LowBrGrZones_m_BGrid_16__m_right
	{0x0F12, 0x019C},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_left
	{0x0F12, 0x01D3},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_right

	{0x002A, 0x0D34},
	{0x0F12, 0x0005},	// #awbb_LowBrGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0012},	// #awbb_LowBrGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0120},	// #awbb_LowBrGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0002},	// #awbb_LowBrGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D48},
	{0x0F12, 0x00E0},	// #awbb_LowBrGrZones_y_high
	{0x0F12, 0x0000},

	// Lowtemp Circle
	{0x0F12, 0x032F},	// #awbb_CrclLowT_R_c
	{0x0F12, 0x0000},
	{0x0F12, 0x017A},	// #awbb_CrclLowT_B_c
	{0x0F12, 0x0000},
	{0x0F12, 0x7300},	// #awbb_CrclLowT_Rad_c
	{0x0F12, 0x0000},
	{0x0F12, 0x000A},	// #awbb_CrclLowT_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D60},
	{0x0F12, 0x00E0},	// #awbb_CrclLowT_y_high
	{0x0F12, 0x0000},
	{0x002A, 0x0D82},
	{0x0F12, 0x0001},	// #awbb_ByPass_LowTempMode

	// White Locus
	{0x002A, 0x0D66},
	{0x0F12, 0x011F},	// #awbb_IntcR
	{0x0F12, 0x0132},	// #awbb_IntcB
	{0x002A, 0x0D74},
	{0x0F12, 0x052A},	// #awbb_MvEq_RBthresh

	// Gamut Thresholds
	{0x002A, 0x0DAE},
	{0x0F12, 0x0036},	// #awbb_GamutWidthThr2
	{0x0F12, 0x001C},	// #awbb_GamutHeightThr2
	{0x002A, 0x0DAA},
	{0x0F12, 0x071A},	// #awbb_GamutWidthThr1
	{0x0F12, 0x03A4},	// #awbb_GamutHeightThr1

	// SceneDetection Thresholds
	{0x002A, 0x0D92},
	{0x0F12, 0x0CE4},	//0A8C	// #awbb_SunnyBr
	{0x0F12, 0x0096},	// #awbb_Sunny_NBzone
	{0x002A, 0x0E86},
	{0x0F12, 0x0216},	// #awbb_OutdoorWP_r
	{0x0F12, 0x029F},	// #awbb_OutdoorWP_b
	{0x002A, 0x0D96},
	{0x0F12, 0x0580},	// #awbb_CloudyBr
	{0x0F12, 0x0096},	// #awbb_Cloudy_NBzone
	{0x002A, 0x0DB2},
	{0x0F12, 0x00DA},	// #awbb_CloudyRB
	{0x002A, 0x0D9A},
	{0x0F12, 0x000A},	// #awbb_Cloudy_BdivRzone
	{0x002A, 0x0DB4},
	{0x0F12, 0x0459},	// #awbb_LowTempRB
	{0x002A, 0x0DA4},
	{0x0F12, 0x000E},	// #awbb_LowTemp_RBzone
	{0x002A, 0x0D64},
	{0x0F12, 0x0040},	// #awbb_DarkBr
	{0x002A, 0x0DA6},
	{0x0F12, 0x001E},	// #awbb_LowBr_NBzone
	{0x002A, 0x0D9C},
	{0x0F12, 0x001B},	// #awbb_MacbethGamut_WidthZone
	{0x0F12, 0x000E},	// #awbb_MacbethGamut_HeightZone
	{0x0F12, 0x0008},	// #awbb_MacbethGamut_WidthZone2
	{0x0F12, 0x0004},	// #awbb_MacbethGamut_HeightZone2

	// AWB Debug.(Outdoor Pink)
	{0x002A, 0x0E30},
	{0x0F12, 0x0000},	//	#awbb_OutdoorFltrSz (outdoor WB를 moving average filtering)

	// UseInvalidOutdoor option
	{0x002A, 0x0D88},
	{0x0F12, 0x0001},	// #awbb_Use_InvalidOutDoor

	// AWB input Y-Filter setting
	{0x002A, 0x0C48},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x002A, 0x0CC4},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high

	{0x002A, 0x0DC2},
	{0x0F12, 0x0030},	// #awbb_GnCurPntImmunity
	{0x0F12, 0x00C8},	// #awbb_GnFarPntImmunity
	{0x0F12, 0x012C},	// #awbb_GnCurPntLongJump
	{0x0F12, 0x0258},	// #awbb_GainsMaxMove
	{0x0F12, 0x0003},	// #awbb_GnMinMatchToJump

	// RB Gain Offset
	//WRITE 70000E32	FFE0	//#awbb_RGainOff
	//WRITE 70000E34	FFe0	//#awbb_BGainOff


	//=================================================================================================
	//	Set CCM
	//=================================================================================================
	// CCM Start Address
	{0x002A, 0x06D0},
	{0x0F12, 0x2800},   //#TVAR_wbt_pBaseCcmsAddr[0]
	{0x0F12, 0x7000},
	{0x0F12, 0x2824},   //#TVAR_wbt_pBaseCcmsAddr[1]
	{0x0F12, 0x7000},
	{0x0F12, 0x2848},   //#TVAR_wbt_pBaseCcmsAddr[2]
	{0x0F12, 0x7000},
	{0x0F12, 0x286C},   //#TVAR_wbt_pBaseCcmsAddr[3]
	{0x0F12, 0x7000},
	{0x0F12, 0x2890},   //#TVAR_wbt_pBaseCcmsAddr[4]
	{0x0F12, 0x7000},
	{0x0F12, 0x28B4},   //#TVAR_wbt_pBaseCcmsAddr[5]
	{0x0F12, 0x7000},
	{0x002A, 0x06EC},
	{0x0F12, 0x28D8},   //#TVAR_wbt_pOutdoorCcm
	{0x0F12, 0x7000},

	{0x002A, 0x2800},
	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	// Outdoor CCM
	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEA4},
	{0x0F12, 0x020F},
	{0x0F12, 0xFF8F},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},



	//=================================================================================================
	//	Set NB
	//=================================================================================================
	{0x002A, 0x07EA},
	{0x0F12, 0x0000},	//afit_bUseNoiseInd 0 : NB 1: Noise Index

	// param_start	SARR_uNormBrInDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrInDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrInDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrInDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrInDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrInDoor[4]

	// param_start	SARR_uNormBrOutDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrOutDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrOutDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrOutDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrOutDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrOutDoor[4]



	//=================================================================================================
	//	Set AFIT
	//=================================================================================================
	// AFIT Start Address
	{0x002A, 0x0814},
	{0x0F12, 0x082C},	//#TVAR_afit_pBaseVals
	{0x0F12, 0x7000},	//#TVAR_afit_pBaseVals

	// AFIT Table	//0223
	{0x002A, 0x082C},
	{0x0F12, 0x0010},	//BRIGHTNESS
	{0x0F12, 0xFFF0},	//CONTRAST
	{0x0F12, 0xFFD0},	//SATURATION
	{0x0F12, 0x0000},	//SHARP_BLUR
	{0x0F12, 0x0000},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0028},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                   bin: desparity low
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"                bin: desparity high
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x06FF},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x030A},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"               ]negati fine
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"                  ]low fine
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"                ]high fine
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"                   ]high low thres
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0005},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01AA},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0006},	//BRIGHTNESS
	{0x0F12, 0x0000},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0000},	//SHARP_BLUR
	{0x0F12, 0x0000},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x000E},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                  bin: desparity low
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x08B7},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0308},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01A0},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x000E},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0089},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0202},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0202},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0206},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x803C},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0006},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0205},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0000},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8050},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0006},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0000},	//UVDenoise_iYLowThresh
	{0x0F12, 0x0000},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0105},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0200},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x805A},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[0]
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[1]
	{0x0F12, 0x0800},	// # afit_pConstBaseVals[2]
	{0x0F12, 0x0600},	// # afit_pConstBaseVals[3]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[4]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[5]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[6]
	{0x0F12, 0x0300},	// # afit_pConstBaseVals[7]
	{0x0F12, 0x0002},	// # afit_pConstBaseVals[8]
	{0x0F12, 0x0400},	// # afit_pConstBaseVals[9]
	{0x0F12, 0x0106},	// # afit_pConstBaseVals[10]
	{0x0F12, 0x0005},	// # afit_pConstBaseVals[11]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[12]
	{0x0F12, 0x0003},	// # afit_pConstBaseVals[13]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[14]
	{0x0F12, 0xFFD6},	// # afit_pConstBaseVals[15]
	{0x0F12, 0x53C1},	// # afit_pConstBaseVals[16]
	{0x0F12, 0xE1FE},	// # afit_pConstBaseVals[17]
	{0x0F12, 0x0001},	// # afit_pConstBaseVals[18]

	{0x002A, 0x01EA},
	{0x0F12, 0x0000},	//#REG_TC_UserSharpBlur
	{0x002A, 0x0E32},
	{0x0F12, 0xFFF0},	//#awbb_RGainOff
	{0x0F12, 0x0000},	//#awbb_BGainOff

	// For fast AE
	{0x002A, 0x0FAC},
	{0x0F12, 0x0001},	// AE Gain output [0]    default : 0x10
	{0x0F12, 0x0008},	// AE Gain output [1]    default : 0x20
	{0x0F12, 0x0015},	// AE Gain output [2]    default : 0x40

	// For fast AWB
	{0x002A, 0x0DCA},
	{0x0F12, 0x0001},	// awbb_GnMinMatchToJump = 0x02 (default:0x03)
	{0x0028, 0x7000},
	{0x002A, 0x0E98},
	{0x0F12, 0x0300},	//bp_uMaxBrightnessFactor 0180 -> 0300
	{0x002A, 0x0E9E},
	{0x0F12, 0x0300},	//bp_uMinBrightnessFactor 0180 -> 0300

	// Update Changed Registers
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},

	{0x0028, 0xD000},
	{0x002A, 0x1088},
	{0x0F12, 0x05D5},

	{0x0028, 0xD000},
	{0x002A, 0x1094},
	{0x0F12, 0x0010},

	{0x0028, 0xD000},
	{0x002A, 0x1082},
	{0x0F12, 0x03FF},

	{0x0028, 0xD000},
	{0x002A, 0x1084},
	{0x0F12, 0x03FF},

    {SEQUENCE_WAIT_MS, 0x0064},
    {SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_init_176x144[] =
{
	{0x0028, 0xD000},
	{0x002A, 0x1030},
	{0x0F12, 0x0000},
	{0x002A, 0x0014},
	{0x0F12, 0x0001},

	{SEQUENCE_WAIT_MS , 0x0014},// Delay 100 -> 20

	{0x0028, 0x7000},
	{0x002A, 0x2080},
	{0x0F12, 0xB510},	// 70002080               0
	{0x0F12, 0xF000},	// 70002082               2
	{0x0F12, 0xF8FC},	// 70002084               4
	{0x0F12, 0xBC10},	// 70002086               6
	{0x0F12, 0xBC08},	// 70002088               8
	{0x0F12, 0x4718},	// 7000208A               A
	{0x0F12, 0xB5FE},	// 7000208C               C
	{0x0F12, 0x0007},	// 7000208E               E
	{0x0F12, 0x683C},	// 70002090               0
	{0x0F12, 0x687E},	// 70002092               2
	{0x0F12, 0x1DA5},	// 70002094               4
	{0x0F12, 0x88A0},	// 70002096               6
	{0x0F12, 0x2800},	// 70002098               8
	{0x0F12, 0xD00B},	// 7000209A               A
	{0x0F12, 0x88A8},	// 7000209C               C
	{0x0F12, 0x2800},	// 7000209E               E
	{0x0F12, 0xD008},	// 700020A0               0
	{0x0F12, 0x8820},	// 700020A2               2
	{0x0F12, 0x8829},	// 700020A4               4
	{0x0F12, 0x4288},	// 700020A6               6
	{0x0F12, 0xD301},	// 700020A8               8
	{0x0F12, 0x1A40},	// 700020AA               A
	{0x0F12, 0xE000},	// 700020AC               C
	{0x0F12, 0x1A08},	// 700020AE               E
	{0x0F12, 0x9001},	// 700020B0               0
	{0x0F12, 0xE001},	// 700020B2               2
	{0x0F12, 0x2019},	// 700020B4               4
	{0x0F12, 0x9001},	// 700020B6               6
	{0x0F12, 0x497B},	// 700020B8               8
	{0x0F12, 0x466B},	// 700020BA               A
	{0x0F12, 0x8A48},	// 700020BC               C
	{0x0F12, 0x8118},	// 700020BE               E
	{0x0F12, 0x8A88},	// 700020C0               0
	{0x0F12, 0x8158},	// 700020C2               2
	{0x0F12, 0x4879},	// 700020C4               4
	{0x0F12, 0x8940},	// 700020C6               6
	{0x0F12, 0x0040},	// 700020C8               8
	{0x0F12, 0x2103},	// 700020CA               A
	{0x0F12, 0xF000},	// 700020CC               C
	{0x0F12, 0xF902},	// 700020CE               E
	{0x0F12, 0x88A1},	// 700020D0               0
	{0x0F12, 0x4288},	// 700020D2               2
	{0x0F12, 0xD908},	// 700020D4               4
	{0x0F12, 0x8828},	// 700020D6               6
	{0x0F12, 0x8030},	// 700020D8               8
	{0x0F12, 0x8868},	// 700020DA               A
	{0x0F12, 0x8070},	// 700020DC               C
	{0x0F12, 0x88A8},	// 700020DE               E
	{0x0F12, 0x6038},	// 700020E0               0
	{0x0F12, 0xBCFE},	// 700020E2               2
	{0x0F12, 0xBC08},	// 700020E4               4
	{0x0F12, 0x4718},	// 700020E6               6
	{0x0F12, 0x88A9},	// 700020E8               8
	{0x0F12, 0x4288},	// 700020EA               A
	{0x0F12, 0xD906},	// 700020EC               C
	{0x0F12, 0x8820},	// 700020EE               E
	{0x0F12, 0x8030},	// 700020F0               0
	{0x0F12, 0x8860},	// 700020F2               2
	{0x0F12, 0x8070},	// 700020F4               4
	{0x0F12, 0x88A0},	// 700020F6               6
	{0x0F12, 0x6038},	// 700020F8               8
	{0x0F12, 0xE7F2},	// 700020FA               A
	{0x0F12, 0x9801},	// 700020FC               C
	{0x0F12, 0xA902},	// 700020FE               E
	{0x0F12, 0xF000},	// 70002100               0
	{0x0F12, 0xF8EE},	// 70002102               2
	{0x0F12, 0x0033},	// 70002104               4
	{0x0F12, 0x0029},	// 70002106               6
	{0x0F12, 0x9A02},	// 70002108               8
	{0x0F12, 0x0020},	// 7000210A               A
	{0x0F12, 0xF000},	// 7000210C               C
	{0x0F12, 0xF8F0},	// 7000210E               E
	{0x0F12, 0x6038},	// 70002110               0
	{0x0F12, 0xE7E6},	// 70002112               2
	{0x0F12, 0xB5F0},	// 70002114               4
	{0x0F12, 0xB08B},	// 70002116               6
	{0x0F12, 0x0006},	// 70002118               8
	{0x0F12, 0x2000},	// 7000211A               A
	{0x0F12, 0x9004},	// 7000211C               C
	{0x0F12, 0x6835},	// 7000211E               E
	{0x0F12, 0x6874},	// 70002120               0
	{0x0F12, 0x68B0},	// 70002122               2
	{0x0F12, 0x900A},	// 70002124               4
	{0x0F12, 0x68F0},	// 70002126               6
	{0x0F12, 0x9009},	// 70002128               8
	{0x0F12, 0x4F60},	// 7000212A               A
	{0x0F12, 0x8979},	// 7000212C               C
	{0x0F12, 0x084A},	// 7000212E               E
	{0x0F12, 0x88A8},	// 70002130               0
	{0x0F12, 0x88A3},	// 70002132               2
	{0x0F12, 0x4298},	// 70002134               4
	{0x0F12, 0xD300},	// 70002136               6
	{0x0F12, 0x0018},	// 70002138               8
	{0x0F12, 0xF000},	// 7000213A               A
	{0x0F12, 0xF8E1},	// 7000213C               C
	{0x0F12, 0x9007},	// 7000213E               E
	{0x0F12, 0x0021},	// 70002140               0
	{0x0F12, 0x0028},	// 70002142               2
	{0x0F12, 0xAA04},	// 70002144               4
	{0x0F12, 0xF000},	// 70002146               6
	{0x0F12, 0xF8E3},	// 70002148               8
	{0x0F12, 0x9006},	// 7000214A               A
	{0x0F12, 0x88A8},	// 7000214C               C
	{0x0F12, 0x2800},	// 7000214E               E
	{0x0F12, 0xD102},	// 70002150               0
	{0x0F12, 0x27FF},	// 70002152               2
	{0x0F12, 0x1C7F},	// 70002154               4
	{0x0F12, 0xE047},	// 70002156               6
	{0x0F12, 0x88A0},	// 70002158               8
	{0x0F12, 0x2800},	// 7000215A               A
	{0x0F12, 0xD101},	// 7000215C               C
	{0x0F12, 0x2700},	// 7000215E               E
	{0x0F12, 0xE042},	// 70002160               0
	{0x0F12, 0x8820},	// 70002162               2
	{0x0F12, 0x466B},	// 70002164               4
	{0x0F12, 0x8198},	// 70002166               6
	{0x0F12, 0x8860},	// 70002168               8
	{0x0F12, 0x81D8},	// 7000216A               A
	{0x0F12, 0x8828},	// 7000216C               C
	{0x0F12, 0x8118},	// 7000216E               E
	{0x0F12, 0x8868},	// 70002170               0
	{0x0F12, 0x8158},	// 70002172               2
	{0x0F12, 0xA802},	// 70002174               4
	{0x0F12, 0xC803},	// 70002176               6
	{0x0F12, 0xF000},	// 70002178               8
	{0x0F12, 0xF8D2},	// 7000217A               A
	{0x0F12, 0x9008},	// 7000217C               C
	{0x0F12, 0x8ABA},	// 7000217E               E
	{0x0F12, 0x9808},	// 70002180               0
	{0x0F12, 0x466B},	// 70002182               2
	{0x0F12, 0x4342},	// 70002184               4
	{0x0F12, 0x9202},	// 70002186               6
	{0x0F12, 0x8820},	// 70002188               8
	{0x0F12, 0x8198},	// 7000218A               A
	{0x0F12, 0x8860},	// 7000218C               C
	{0x0F12, 0x81D8},	// 7000218E               E
	{0x0F12, 0x980A},	// 70002190               0
	{0x0F12, 0x9903},	// 70002192               2
	{0x0F12, 0xF000},	// 70002194               4
	{0x0F12, 0xF8C4},	// 70002196               6
	{0x0F12, 0x9A02},	// 70002198               8
	{0x0F12, 0x17D1},	// 7000219A               A
	{0x0F12, 0x0E09},	// 7000219C               C
	{0x0F12, 0x1889},	// 7000219E               E
	{0x0F12, 0x1209},	// 700021A0               0
	{0x0F12, 0x4288},	// 700021A2               2
	{0x0F12, 0xDD1F},	// 700021A4               4
	{0x0F12, 0x8820},	// 700021A6               6
	{0x0F12, 0x466B},	// 700021A8               8
	{0x0F12, 0x8198},	// 700021AA               A
	{0x0F12, 0x8860},	// 700021AC               C
	{0x0F12, 0x81D8},	// 700021AE               E
	{0x0F12, 0x980A},	// 700021B0               0
	{0x0F12, 0x9903},	// 700021B2               2
	{0x0F12, 0xF000},	// 700021B4               4
	{0x0F12, 0xF8B4},	// 700021B6               6
	{0x0F12, 0x9001},	// 700021B8               8
	{0x0F12, 0x8828},	// 700021BA               A
	{0x0F12, 0x466B},	// 700021BC               C
	{0x0F12, 0x8118},	// 700021BE               E
	{0x0F12, 0x8868},	// 700021C0               0
	{0x0F12, 0x8158},	// 700021C2               2
	{0x0F12, 0x980A},	// 700021C4               4
	{0x0F12, 0x9902},	// 700021C6               6
	{0x0F12, 0xF000},	// 700021C8               8
	{0x0F12, 0xF8AA},	// 700021CA               A
	{0x0F12, 0x8AB9},	// 700021CC               C
	{0x0F12, 0x9A08},	// 700021CE               E
	{0x0F12, 0x4351},	// 700021D0               0
	{0x0F12, 0x17CA},	// 700021D2               2
	{0x0F12, 0x0E12},	// 700021D4               4
	{0x0F12, 0x1851},	// 700021D6               6
	{0x0F12, 0x120A},	// 700021D8               8
	{0x0F12, 0x9901},	// 700021DA               A
	{0x0F12, 0xF000},	// 700021DC               C
	{0x0F12, 0xF890},	// 700021DE               E
	{0x0F12, 0x0407},	// 700021E0               0
	{0x0F12, 0x0C3F},	// 700021E2               2
	{0x0F12, 0xE000},	// 700021E4               4
	{0x0F12, 0x2700},	// 700021E6               6
	{0x0F12, 0x8820},	// 700021E8               8
	{0x0F12, 0x466B},	// 700021EA               A
	{0x0F12, 0xAA05},	// 700021EC               C
	{0x0F12, 0x8198},	// 700021EE               E
	{0x0F12, 0x8860},	// 700021F0               0
	{0x0F12, 0x81D8},	// 700021F2               2
	{0x0F12, 0x8828},	// 700021F4               4
	{0x0F12, 0x8118},	// 700021F6               6
	{0x0F12, 0x8868},	// 700021F8               8
	{0x0F12, 0x8158},	// 700021FA               A
	{0x0F12, 0xA802},	// 700021FC               C
	{0x0F12, 0xC803},	// 700021FE               E
	{0x0F12, 0x003B},	// 70002200               0
	{0x0F12, 0xF000},	// 70002202               2
	{0x0F12, 0xF895},	// 70002204               4
	{0x0F12, 0x88A1},	// 70002206               6
	{0x0F12, 0x88A8},	// 70002208               8
	{0x0F12, 0x003A},	// 7000220A               A
	{0x0F12, 0xF000},	// 7000220C               C
	{0x0F12, 0xF898},	// 7000220E               E
	{0x0F12, 0x0004},	// 70002210               0
	{0x0F12, 0xA804},	// 70002212               2
	{0x0F12, 0xC803},	// 70002214               4
	{0x0F12, 0x9A09},	// 70002216               6
	{0x0F12, 0x9B07},	// 70002218               8
	{0x0F12, 0xF000},	// 7000221A               A
	{0x0F12, 0xF889},	// 7000221C               C
	{0x0F12, 0xA806},	// 7000221E               E
	{0x0F12, 0xC805},	// 70002220               0
	{0x0F12, 0x0021},	// 70002222               2
	{0x0F12, 0xF000},	// 70002224               4
	{0x0F12, 0xF88C},	// 70002226               6
	{0x0F12, 0x6030},	// 70002228               8
	{0x0F12, 0xB00B},	// 7000222A               A
	{0x0F12, 0xBCF0},	// 7000222C               C
	{0x0F12, 0xBC08},	// 7000222E               E
	{0x0F12, 0x4718},	// 70002230               0
	{0x0F12, 0xB510},	// 70002232               2
	{0x0F12, 0x6800},	// 70002234               4
	{0x0F12, 0x2805},	// 70002236               6
	{0x0F12, 0xD11E},	// 70002238               8
	{0x0F12, 0x481D},	// 7000223A               A
	{0x0F12, 0x491D},	// 7000223C               C
	{0x0F12, 0x8800},	// 7000223E               E
	{0x0F12, 0x8348},	// 70002240               0
	{0x0F12, 0x2301},	// 70002242               2
	{0x0F12, 0x31A0},	// 70002244               4
	{0x0F12, 0x720B},	// 70002246               6
	{0x0F12, 0x2000},	// 70002248               8
	{0x0F12, 0x000A},	// 7000224A               A
	{0x0F12, 0x3A60},	// 7000224C               C
	{0x0F12, 0x7150},	// 7000224E               E
	{0x0F12, 0x71CB},	// 70002250               0
	{0x0F12, 0x22FF},	// 70002252               2
	{0x0F12, 0x3980},	// 70002254               4
	{0x0F12, 0x75CA},	// 70002256               6
	{0x0F12, 0x764A},	// 70002258               8
	{0x0F12, 0x000A},	// 7000225A               A
	{0x0F12, 0x3260},	// 7000225C               C
	{0x0F12, 0x7210},	// 7000225E               E
	{0x0F12, 0x7250},	// 70002260               0
	{0x0F12, 0x2008},	// 70002262               2
	{0x0F12, 0x7488},	// 70002264               4
	{0x0F12, 0x74C8},	// 70002266               6
	{0x0F12, 0x4813},	// 70002268               8
	{0x0F12, 0x217E},	// 7000226A               A
	{0x0F12, 0x43C9},	// 7000226C               C
	{0x0F12, 0x8101},	// 7000226E               E
	{0x0F12, 0x2306},	// 70002270               0
	{0x0F12, 0x5EC0},	// 70002272               2
	{0x0F12, 0xF000},	// 70002274               4
	{0x0F12, 0xF86C},	// 70002276               6
	{0x0F12, 0xBC10},	// 70002278               8
	{0x0F12, 0xBC08},	// 7000227A               A
	{0x0F12, 0x4718},	// 7000227C               C
	{0x0F12, 0xB570},	// 7000227E               E
	{0x0F12, 0x2200},	// 70002280               0
	{0x0F12, 0x490E},	// 70002282               2
	{0x0F12, 0x480E},	// 70002284               4
	{0x0F12, 0x2401},	// 70002286               6
	{0x0F12, 0xF000},	// 70002288               8
	{0x0F12, 0xF86A},	// 7000228A               A
	{0x0F12, 0x0022},	// 7000228C               C
	{0x0F12, 0x490D},	// 7000228E               E
	{0x0F12, 0x480D},	// 70002290               0
	{0x0F12, 0x2502},	// 70002292               2
	{0x0F12, 0xF000},	// 70002294               4
	{0x0F12, 0xF864},	// 70002296               6
	{0x0F12, 0x490C},	// 70002298               8
	{0x0F12, 0x480D},	// 7000229A               A
	{0x0F12, 0x002A},	// 7000229C               C
	{0x0F12, 0xF000},	// 7000229E               E
	{0x0F12, 0xF85F},	// 700022A0               0
	{0x0F12, 0xBC70},	// 700022A2               2
	{0x0F12, 0xBC08},	// 700022A4               4
	{0x0F12, 0x4718},	// 700022A6               6
	{0x0F12, 0x1A28},	// 700022A8               8
	{0x0F12, 0x7000},	// 700022AA               A
	{0x0F12, 0x0D64},	// 700022AC               C
	{0x0F12, 0x7000},	// 700022AE               E
	{0x0F12, 0x2370},	// 700022B0               0
	{0x0F12, 0x7000},	// 700022B2               2
	{0x0F12, 0x1C14},	// 700022B4               4
	{0x0F12, 0x7000},	// 700022B6               6
	{0x0F12, 0x14FA},	// 700022B8               8
	{0x0F12, 0x7000},	// 700022BA               A
	{0x0F12, 0x2115},	// 700022BC               C
	{0x0F12, 0x7000},	// 700022BE               E
	{0x0F12, 0x622F},	// 700022C0               0
	{0x0F12, 0x0000},	// 700022C2               2
	{0x0F12, 0x208D},	// 700022C4               4
	{0x0F12, 0x7000},	// 700022C6               6
	{0x0F12, 0x6445},	// 700022C8               8
	{0x0F12, 0x0000},	// 700022CA               A
	{0x0F12, 0x2233},	// 700022CC               C
	{0x0F12, 0x7000},	// 700022CE               E
	{0x0F12, 0x3B5B},	// 700022D0               0
	{0x0F12, 0x0000},	// 700022D2               2
	{0x0F12, 0x4778},	// 700022D4               4
	{0x0F12, 0x46C0},	// 700022D6               6
	{0x0F12, 0xF004},	// 700022D8               8
	{0x0F12, 0xE51F},	// 700022DA               A
	{0x0F12, 0xA464},	// 700022DC               C
	{0x0F12, 0x0000},	// 700022DE               E
	{0x0F12, 0x4778},	// 700022E0               0
	{0x0F12, 0x46C0},	// 700022E2               2
	{0x0F12, 0xC000},	// 700022E4               4
	{0x0F12, 0xE59F},	// 700022E6               6
	{0x0F12, 0xFF1C},	// 700022E8               8
	{0x0F12, 0xE12F},	// 700022EA               A
	{0x0F12, 0x6009},	// 700022EC               C
	{0x0F12, 0x0000},	// 700022EE               E
	{0x0F12, 0x4778},	// 700022F0               0
	{0x0F12, 0x46C0},	// 700022F2               2
	{0x0F12, 0xC000},	// 700022F4               4
	{0x0F12, 0xE59F},	// 700022F6               6
	{0x0F12, 0xFF1C},	// 700022F8               8
	{0x0F12, 0xE12F},	// 700022FA               A
	{0x0F12, 0x622F},	// 700022FC               C
	{0x0F12, 0x0000},	// 700022FE               E
	{0x0F12, 0x4778},	// 70002300               0
	{0x0F12, 0x46C0},	// 70002302               2
	{0x0F12, 0xC000},	// 70002304               4
	{0x0F12, 0xE59F},	// 70002306               6
	{0x0F12, 0xFF1C},	// 70002308               8
	{0x0F12, 0xE12F},	// 7000230A               A
	{0x0F12, 0x5F49},	// 7000230C               C
	{0x0F12, 0x0000},	// 7000230E               E
	{0x0F12, 0x4778},	// 70002310               0
	{0x0F12, 0x46C0},	// 70002312               2
	{0x0F12, 0xC000},	// 70002314               4
	{0x0F12, 0xE59F},	// 70002316               6
	{0x0F12, 0xFF1C},	// 70002318               8
	{0x0F12, 0xE12F},	// 7000231A               A
	{0x0F12, 0x5FC7},	// 7000231C               C
	{0x0F12, 0x0000},	// 7000231E               E
	{0x0F12, 0x4778},	// 70002320               0
	{0x0F12, 0x46C0},	// 70002322               2
	{0x0F12, 0xC000},	// 70002324               4
	{0x0F12, 0xE59F},	// 70002326               6
	{0x0F12, 0xFF1C},	// 70002328               8
	{0x0F12, 0xE12F},	// 7000232A               A
	{0x0F12, 0x5457},	// 7000232C               C
	{0x0F12, 0x0000},	// 7000232E               E
	{0x0F12, 0x4778},	// 70002330               0
	{0x0F12, 0x46C0},	// 70002332               2
	{0x0F12, 0xC000},	// 70002334               4
	{0x0F12, 0xE59F},	// 70002336               6
	{0x0F12, 0xFF1C},	// 70002338               8
	{0x0F12, 0xE12F},	// 7000233A               A
	{0x0F12, 0x5FA3},	// 7000233C               C
	{0x0F12, 0x0000},	// 7000233E               E
	{0x0F12, 0x4778},	// 70002340               0
	{0x0F12, 0x46C0},	// 70002342               2
	{0x0F12, 0xC000},	// 70002344               4
	{0x0F12, 0xE59F},	// 70002346               6
	{0x0F12, 0xFF1C},	// 70002348               8
	{0x0F12, 0xE12F},	// 7000234A               A
	{0x0F12, 0x51F9},	// 7000234C               C
	{0x0F12, 0x0000},	// 7000234E               E
	{0x0F12, 0x4778},	// 70002350               0
	{0x0F12, 0x46C0},	// 70002352               2
	{0x0F12, 0xC000},	// 70002354               4
	{0x0F12, 0xE59F},	// 70002356               6
	{0x0F12, 0xFF1C},	// 70002358               8
	{0x0F12, 0xE12F},	// 7000235A               A
	{0x0F12, 0x7633},	// 7000235C               C
	{0x0F12, 0x0000},	// 7000235E               E
	{0x0F12, 0x4778},	// 70002360               0
	{0x0F12, 0x46C0},	// 70002362               2
	{0x0F12, 0xC000},	// 70002364               4
	{0x0F12, 0xE59F},	// 70002366               6
	{0x0F12, 0xFF1C},	// 70002368               8
	{0x0F12, 0xE12F},	// 7000236A               A
	{0x0F12, 0xA007},	// 7000236C               C
	{0x0F12, 0x0000},	// 7000236E               E
	{0x0F12, 0x0360},	// 70002370               0
	{0x0F12, 0x6F4E},	// 70002372               2
	{0x0F12, 0x2076},	// 70002374               4
	{0x0F12, 0x3831},	// 70002376               6
	{0x0F12, 0x3220},	// 70002378               8
	{0x0F12, 0x3030},	// 7000237A               A
	{0x0F12, 0x0038},	// 7000237C               C
	{0x0F12, 0xE010},	// 7000237E               E
	{0x0F12, 0x0208},	// 70002380               0
	{0x0F12, 0x0038},	// 70002382               2
	// End of Trap and Patch (Last : 70002382h) // Total Size 772 (0x0304)
	{0x0028, 0xD000},
	{0x002A, 0x1000},
	{0x0F12, 0x0001},

	{0x0028, 0x7000},
	{0x002A, 0x0ADC},
	{0x0F12, 0x0AF0},	//#setot_uOnlineClocksDiv4OnlineC0             //2800
	{0x002A, 0x0AE2},
	{0x0F12, 0x222E},	//#setot_usSetRomWaitStatesSetRomThreshold4KHz //ROM Wait Threshold to 35MHz
	{0x002A, 0x0B94},
	{0x0F12, 0x0500},	//#awbb_GainsInit_0_:R    insInit
	{0x0F12, 0x0400},	//#awbb_GainsInit_1_:G    insInit
	{0x0F12, 0x0700},	//#awbb_GainsInit_2_:B    insInit
	{0x002A, 0x04A0},
	{0x0F12, 0x8000},	//#lt_uLeiInit:AE start   Init:AE
	{0x002A, 0x049A},
	{0x0F12, 0x003B},	//#lt_uMinExp             Exp



	//=================================================================================================
	//	Set CIS/APS/Analog
	//=================================================================================================
	// This registers are for FACTORY ONLY. If you change it without prior notification,
	// YOU are RESPONSIBLE for the FAILURE that will happen in the future.
	//=================================================================================================
	{0x0028, 0xD000},
	{0x002A, 0xF106},
	{0x0F12, 0x0001},	// L-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xF206},
	{0x0F12, 0x0001},	// F-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xC202},
	{0x0F12, 0x0700},	// tgr_coarse_integration_time[15:0]: revised by Ana 080115
	{0x002A, 0xF414},
	{0x0F12, 0x0030},	// aig_adc_sat[7:0] (aig_adc_sat[7:4]+1)*33mV + 690mV
	{0x002A, 0xC204},
	{0x0F12, 0x0100},	// tgr_analogue_code_global[12:0] Analog gain X8
	{0x002A, 0xF402},
	{0x0F12, 0x0092},	// aig_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_offset2[7:0]: revised by Ana 080425
	{0x002A, 0xF700},
	{0x0F12, 0x0040},	// bpradlc_control[7:0]: revised by Ana 080529 // bpradlc_nactive_pedestal[7:5],bpradlc_passthrough[1],bpradlc_bypass[0]
	{0x002A, 0xF708},
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_r[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gr[7:0]: revised by Tg 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gb[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_b[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_total[7:0]: revised by TG 080529
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_r[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gr[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gb[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_b[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_total[7:0]: revised by Ana 080425
	{0x0F12, 0x0001},	// bpradlc_adlc_en[7:0]: revised by TG 080529	// bpradlc_f_l_adlc_en[2],bpradlc_f_adlc_en[1],bpradlc_h_adlc_en[0]
	{0x0F12, 0x0015},	// bpradlc_adlc_option[7:0]: revised by TG 080425 // bpradlc_h_adlc_ch_sel[4], // bpradlc_max_data_clip_sel[3:2],bpradlc_f_adlc_max_data_clip_sel[1:0]
	{0x0F12, 0x0001},	// bpradlc_adlc_bpr_en[7:0] adlc_bpr_enable: revised by Ana 080112
	{0x0F12, 0x0040},	// bpradlc_adlc_bpr_thresh[7:0]: 080425
	{0x002A, 0xF48A},
	{0x0F12, 0x0048},	// aig_ld_ptr7[9:0] line OB start addr(04h~48h): revised by Ana 080911
	{0x002A, 0xF10A},
	{0x0F12, 0x008B},	// tgr_h_desc0_addr_end[10:0] line OB end addr(47h~8Bh): revised by Ana 080911 // line OB start - line OB end = 67d
	{0x002A, 0xF900},
	{0x0F12, 0x0067},	// cfpn_config: revised by Ana 080425 // cfpn_ref_gain[9:7],cpfn_ref_lines[6:5],cfpn_hbinning[4],cfpn_output_direct[3],// cfpn_overflow_protect[2],cpfn_passthrough[1],cfpn_bypass[0]
	{0x002A, 0xF406},
	{0x0F12, 0x0092},	// aig_cfpn_ref_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_cfpn_ref_offset2[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_cfpn_ref_gain[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_ld_ctrl[1:0] aig_ldb_en[1], aig_ld_en[0]
	{0x0F12, 0x0000},	// aig_row_id_ctrl[1:0]
	{0x002A, 0xF442},
	{0x0F12, 0x0000},	// aig_vavg[0]
	{0x0F12, 0x0000},	// aig_havg[0]=1b @H-avg mode: revised by Ana 080116
	{0x002A, 0xF448},
	{0x0F12, 0x0000},	// aig_sl_off[0]
	{0x002A, 0xF454},
	{0x0F12, 0x0001},	//Power save enable
	{0x0F12, 0x0001},	// aig_blst_en[0]
	{0x0F12, 0x0010},	// aig_blst_en_cintr[15:0]
	{0x0F12, 0x0000},	// aig_dshut_en[0]
	{0x002A, 0xF41A},
	{0x0F12, 0x00FF},	// aig_comp_bias[7:0] aig_comp2_bias[7:4], aig_comp1_bias[3:0]: revised by Ana 081013
	{0x0F12, 0x0003},	// aig_pix_bias[3:0]
	{0x002A, 0xF420},
	{0x0F12, 0x0030},	// aig_clp_lvl[7:0]: revised by Ana 080910(refer to 6AA)
	{0x002A, 0xF410},
	{0x0F12, 0x0001},	// aig_clp_sl_ctrl[0]
	{0x0F12, 0x0000},	// aig_cds_test[7:0], aig_cds_test[1]=1b @H-avg mode current save: revised by Ana 080116
	{0x002A, 0xF416},
	{0x0F12, 0x0001},	// aig_rmp_option[7:0]
	{0x002A, 0xF424},
	{0x0F12, 0x0000},	// aig_ref_option[7:0], aig_ref_option[0]=1b @H-avg mode odd cds off: revised by Ana 080116
	{0x002A, 0xF422},
	{0x0F12, 0x0000},	// aig_monit[7:0]
	{0x002A, 0xF41E},
	{0x0F12, 0x0000},	// aig_pd_pix[0]
	{0x002A, 0xF428},
	{0x0F12, 0x0000},	// aig_pd_cp[1:0] aig_pd_ncp[1], aig_pd_cp[0]
	{0x0F12, 0x0000},	// aig_pd_reg_pix[0]
	{0x0F12, 0x0000},	// aig_pd_reg_rg[0]
	{0x002A, 0xF430},
	{0x0F12, 0x0000},	// aig_pd_reg_tgsl[0]
	{0x0F12, 0x0000},	// aig_pd_reg_ntg[0]
	{0x0F12, 0x0008},	// aig_rosc_tune_cp[3:0]: revised by Ana 080418
	{0x0F12, 0x0005},	// aig_rosc_tune_ncp[3:0]
	{0x0F12, 0x000F},	// aig_cp_capa[3:0]
	{0x0F12, 0x0001},	// aig_reg_tune_pix[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_rg[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_tgsl[7:0]
	{0x0F12, 0x0010},	// aig_reg_tune_ntg[7:0] 0040 -> 0010
	{0x002A, 0xF4D6},
	{0x0F12, 0x00C0},	// aig_v15_tune[7:0]: revised by Ana 081010 // aig_v15_tune[7:4]=7h @sensor only Mode // aig_v15_tune[7:4]=9h @ISP 7.5fps Mode // aig_v15_tune[7:4]=Dh @ISP 15fps Mode
	{0x0F12, 0x0000},	//aig_vreg_sel[7:0] [9]h_test, [1]reg_sw_stby, [0]aig_reg_sel
	{0x002A, 0xF47C},
	{0x0F12, 0x000C},	//aig_ld_ptr0[4:0]
	{0x0F12, 0x0000},	//aig_ld_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF49A},
	{0x0F12, 0x0008},	//aig_sla_ptr0[3:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_sla_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4A2},
	{0x0F12, 0x0008},	//aig_slb_ptr0[7:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_slb_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4B2},
	{0x0F12, 0x0013},	//aig_rxa_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxa_ptr1[9:0]: revised by Ana 081010
	{0x0F12, 0x0013},	//aig_rxb_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxb_ptr1[9:0]: revised by Ana 081010
	{0x002A, 0xF4AA},
	{0x0F12, 0x009B},	//aig_txa_ptr0[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txa_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_txb_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txb_ptr1[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF474},
	{0x0F12, 0x0017},	//aig_s3_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x005F},	//aig_s3_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0017},	//aig_s4_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x008F},	//aig_s4_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF48C},
	{0x0F12, 0x0017},	//aig_clp_sl_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_clp_sl_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF4C8},
	{0x0F12, 0x0163},	//aig_off_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0193},	//aig_rmp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF490},
	{0x0F12, 0x0191},	//aig_comp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF418},
	{0x0F12, 0x0083},	//aig_dbs_option[7:0]: revised by Ana 081010

	//For ESD Check
	{0x0028, 0x7000},
	{0x002A, 0x0132},
	{0x0F12, 0xAAAA},	//#define REG_FWpid (For ESD Check)
	// * note : ESD Check Method.
	//	1. if (7000.0132 != AAAA ) reset // ( S/W reg. check)
	//	2. if (D000.1002 != 0000 ) reset // (internal Exception)

	//Set FPN Gain Input
	{0x002A, 0x1176},
	{0x0F12, 0x0020},	// fpn_GainInput[0]
	{0x0F12, 0x0040},	// fpn_GainInput[1]
	{0x0F12, 0x0080},	// fpn_GainInput[2]
	{0x0F12, 0x0100},	// fpn_GainInput[3]
	{0x0F12, 0x0014},	// fpn_GainOutput[0]
	{0x0F12, 0x000A},	// fpn_GainOutput[1]
	{0x0F12, 0x0008},	// fpn_GainOutput[2]
	{0x0F12, 0x0004},	// fpn_GainOutput[3]

	// Set CFPN Canceller
	{0x002A, 0x116E},
	{0x0F12, 0x0000},	// OvflProtect
	{0x0F12, 0x0000},	// bypassThrough
	{0x0F12, 0x0000},	// bypass
	{0x0F12, 0x0002},	// fpn.CollectMethod	// 0 : Center	1 : Right	2: LEFT
	{0x002A, 0x0AE8},
	{0x0F12, 0x0000},	// setot_bSendFpnToISP = ??

	// Set Forbidden Area
	{0x002A, 0x10EE},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_usRegCount // 0 value --> not use aig table 3
	{0x002A, 0x10F2},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_pSenModesRegsArray3[0]		//real register 0xD000F45A
	{0x002A, 0x1152},
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[48]    //real register 0xD000F4BA
	{0x0F12, 0x0028},	// senHal_SenRegsModes3_pSenModesRegsArray3[49]    //real register 0xD000F4BC
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[50]    //real register 0xD000F4BE
	{0x002A, 0x1148},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[43]    //real register 0xD000F4B0
	{0x002A, 0x1144},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[41]    //real register 0xD000F4AC
	{0x002A, 0x1150},
	{0x0F12, 0x0148},	// senHal_SenRegsModes3_pSenModesRegsArray3[47]    //real register 0xD000F4B8

	// Set AE Target
	{0x002A, 0x0F4C},
	{0x0F12, 0x003D},	// #TVAR_ae_BrAve



	//=================================================================================================
	//	Set Frame Rate
	//=================================================================================================
	{0x002A, 0x0484},
	{0x0F12, 0x61A8},	// #lt_uMaxExp1 	// 50ms
	{0x0F12, 0x0000},
	{0x0F12, 0xE848},	// #lt_uMaxExp2 	// 250ms
	{0x0F12, 0x0001},
	{0x0F12, 0x03C4},	// #lt_uCapMaxExp1	// 133ms
	{0x0F12, 0x0001},
	{0x0F12, 0xE848},	// #lt_uCapMaxExp2	// 250ms
	{0x0F12, 0x0001},
	{0x0F12, 0x04C0},	// #lt_uMaxAnGain1	// x4.75
	{0x0F12, 0x0700},	// #lt_uMaxAnGain2	// x7.0
	{0x0F12, 0x0100},	// #lt_uMaxDigGain
	{0x002A, 0x0F52},
	{0x0F12, 0x0007},	// #ae_StatMode



	//=================================================================================================
	//	Set Flicker  Current Flicker = 7000.164A //Mon_AFC_usFlExpQunat		0001:Off 1045:60Hz	1388:50Hz
	//=================================================================================================

	//1. Auto Flicker 60Hz Start
	{0x002A, 0x0B2E},
	{0x0F12, 0x0001},	// #AFC_Default60Hz 	1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	{0x002A, 0x03F8},
	{0x0F12, 0x007F},	// #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////2. Auto Flicker 50Hz Start
	//WRITE	70000B2E	0000	// #AFC_Default60Hz		1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	//WRITE 700003F8	007F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////3. Manual Flicker 60Hz


	//=================================================================================================
	//	Set PLL
	//=================================================================================================
	// Input CLK
	{0x002A, 0x01B8},
	{0x0F12, 0x5DC0},	//5DC0	//#REG_TC_IPRM_InClockLSBs	//24 -> 24.5M Hz(20081208)
	{0x0F12, 0x0000},	//#REG_TC_IPRM_InClockMSBs

	// Target Sys CLK
	{0x002A, 0x01CC},
	{0x0F12, 0x1770},	// #REG_TC_IPRM_OpClk4KHz_0 			//24Mhz

	// Target PVI CLK
	{0x0F12, 0x2318},	// #REG_TC_IPRM_MinOutRate4KHz_0	//36Mhz
	{0x0F12, 0x2338},	// #REG_TC_IPRM_MaxOutRate4KHz_0

	// PLL ETC
	{0x002A, 0x01C6},
	{0x0F12, 0x0003},	//#REG_TC_IPRM_UseNPviClocks
	{0x0F12, 0x0000},	//#REG_TC_IPRM_UseNMipiClocks
	{0x002A, 0x01C0},
	{0x0F12, 0x0000},	//#EG_TC_IPRM_ValidVActiveLow
	{0x002A, 0x01DE},
	{0x0F12, 0x0001},	//#REG_TC_IPRM_UseRegsAPI
	{0x0F12, 0x0001},	//#REG_TC_IPRM_InitParamsUpdated
	{0x002A, 0x1084},
	{0x0F12, 0x0001},	//#senHal_bUseAnalogBinning //Avg S.S
	{0x002A, 0x10B6},
	{0x0F12, 0x0048},	//#senHal_uMinColsAddAnalogBin



	//=================================================================================================
	//	Set Preview Config
	//=================================================================================================

	//* Preview Cropping Code *//
#ifdef VC_CAMERA_CROP
	{0x002A,	0x020A},
//	{0x0F12,	0x0640},	//#REG_TC_GP_PrevZoomReqInputWidth
//	{0x0F12,	0x0384},	//#REG_TC_GP_PrevZoomReqInputHeight
	{0x0F12,	0x0500},	//#REG_TC_GP_PrevZoomReqInputWidthOfs		0x500(1280)
	{0x0F12,	0x0418},	//#REG_TC_GP_PrevZoomReqInputHeightOfs		0x418(1048)

	//* Zoom Update *//
	{0x0F12,	0x0000},	//#REG_TC_GP_PrevZoomReqInputWidthOffset
	{0x0F12,	0x0098},	//#REG_TC_GP_PrevZoomReqInputHeightOffset

	{0x002A,	0x021A},
	{0x0F12,	0x0001},	// #REG_TC_GP_Inputs_ChangeRequest : 0x021A
#endif
	{0x002A, 0x0242},
	{0x0F12, 0x00B0},  // #REG_0TC_PCFG_usWidth			// 0x160 => 352, 0x280 => 640, 0xB0 => 176
	{0x0F12, 0x0090},  // #REG_0TC_PCFG_usHeight			// 0x120 => 288, 0x1E0 => 480, 0x90 => 144
	{0x0F12, 0x0005},	// #REG_0TC_PCFG_Format 						// YUV output
	{0x0F12, 0x2338},	// #REG_0TC_PCFG_usMaxOut4KHzRate
	{0x0F12, 0x2318},	// #REG_0TC_PCFG_usMinOut4KHzRate
//	{0x0F12, 0x446C},	// #REG_0TC_PCFG_usMaxOut4KHzRate
//	{0x0F12, 0x444C},	// #REG_0TC_PCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	// #REG_0TC_PCFG_PVIMask   (x2: PCLK rising x0: PCLK Falling)
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_uClockInd
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_usFrTimeType
//	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType		(0 : bin mode, 2 : No-bin mode)
	{0x0F12, 0x02AF},	//03E8},	// #REG_0TC_PCFG_usMaxFrTimeMsecMult10 // 10fps --> 15fps
	{0x0F12, 0x01F4},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 20fps
//	{0x0F12, 0x014D},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 30fps
	{0x002A, 0x0262},
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uPrevMirror -- X/Y flip 20090112
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uCaptureMirror -- X/Y flip 20090112
	//WRITE 70000262 000F  //#REG_0TC_PCFG_uPrevMirror
	//WRITE 70000264 000F  //#REG_0TC_PCFG_uCaptureMirror


	// Apply preview config
	{0x002A, 0x021C},
	{0x0F12, 0x0000},	//#REG_TC_GP_ActivePrevConfig	  //preview config0
	{0x002A, 0x0220},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevOpenAfterChange  //config 변경적용
	{0x002A, 0x01F8},
	{0x0F12, 0x0001},	//#REG_TC_GP_NewConfigSync
	{0x002A, 0x021E},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevConfigChanged



	//=================================================================================================
	//	Set Capture Config
	//=================================================================================================
	//WRITE 70000214	04C0	// #REG_TC_GP_CapZoomReqInputHeight	// Vsize = 1216
	//WRITE 70000216	0000	// #REG_TC_GP_CapZoomReqInputWidthOfs
	//WRITE 70000218	0000	// #REG_TC_GP_CapZoomReqInputHeightOfs
	//WRITE 7000021A	0001	// #REG_TC_GP_InputsChangeRequest
	//WRITE 700001F8	0001	// #REG_TC_GP_NewConfigSync
	//
#ifdef VC_CAMERA_CROP
	//* Capture Cropping Code *//
	{0x0028, 0x7000},
	{0x002A, 0x0212},
	{0x0F12, 0x0500}, //#REG_TC_GP_CapZoomReqInputWidth
	{0x0F12, 0x0418}, //#REG_TC_GP_CapZoomReqInputHeight
	{0x0F12, 0x0000}, //#REG_TC_GP_CapZoomReqInputWidthOfs
	{0x0F12, 0x0098}, //#REG_TC_GP_CapZoomReqInputHeightOfs
#endif
	{0x002A, 0x0302},
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uCaptureMode
	{0x0F12, 0x0500},	//#REG_0TC_CCFG_usWidth
	{0x0F12, 0x0418},	//#REG_0TC_CCFG_usHeight						// Vsize = 1216
	{0x0F12, 0x0005},	//#REG_0TC_CCFG_Format	   //5:YUV,9:JPEG
	{0x0F12, 0x2338},	//#REG_0TC_CCFG_usMaxOut4KHzRate
	{0x0F12, 0x2318},	//#REG_0TC_CCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	//#REG_0TC_CCFG_PVIMask (x2: PCLK rising	x0: PCLK Falling)
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uClockInd
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_usFrTimeType
	{0x0F12, 0x0002},	//#REG_0TC_CCFG_FrRateQualityType     (0 : bin mode, 2 : No-bin mode)
	{0x0F12, 0x09C4},	//07D0},	//#REG_0TC_CCFG_usMaxFrTimeMsecMult10 // 5fps --> 4fps
	{0x0F12, 0x0535},	//#REG_0TC_CCFG_usMinFrTimeMsecMult10 // 7.5fps

	//* Capture Configuration Update *//
	{0x002A, 0x01F8},
	{0x0F12, 0x0001}, //REG_TC_GP_NewConfigSync 0001 : update configuration
	{0x002a, 0x0226},
	{0x0f12, 0x0001}, //REG_TC_GP_CapConfigChanged 0001
	//=================================================================================================
	//	Set Periodic Mismatch (Use default value)
	//=================================================================================================



	//=================================================================================================
	//	Set AE Weights
	//=================================================================================================
	{0x002A, 0x0F5A},
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_0_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_1_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_2_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_3_
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_4_
	{0x0F12, 0x0101},	//0201	// #ae_WeightTbl_16_5_
	{0x0F12, 0x0100},	//0102	// #ae_WeightTbl_16_6_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_7_
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_8_
	{0x0F12, 0x0202},	//0202	// #ae_WeightTbl_16_9_
	{0x0F12, 0x0101},	//0202	// #ae_WeightTbl_16_10
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_11
	{0x0F12, 0x0201},	//0101	// #ae_WeightTbl_16_12
	{0x0F12, 0x0303},	//0402	// #ae_WeightTbl_16_13
	{0x0F12, 0x0102},	//0204	// #ae_WeightTbl_16_14
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_15
	{0x0F12, 0x0201},	//0201	// #ae_WeightTbl_16_16
	{0x0F12, 0x0505},	//0504	// #ae_WeightTbl_16_17
	{0x0F12, 0x0102},	//0405	// #ae_WeightTbl_16_18
	{0x0F12, 0x0000},	//0102	// #ae_WeightTbl_16_19
	{0x0F12, 0x0201},	//0501	// #ae_WeightTbl_16_20
	{0x0F12, 0x0303},	//0505	// #ae_WeightTbl_16_21
	{0x0F12, 0x0102},	//0505	// #ae_WeightTbl_16_22
	{0x0F12, 0x0000},	//0105	// #ae_WeightTbl_16_23
	{0x0F12, 0x0101},	//0503	// #ae_WeightTbl_16_24
	{0x0F12, 0x0202},	//0505	// #ae_WeightTbl_16_25
	{0x0F12, 0x0101},	//0505	// #ae_WeightTbl_16_26
	{0x0F12, 0x0000},	//0305	// #ae_WeightTbl_16_27
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_28
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_29
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_30
	{0x0F12, 0x0000},	//0303	// #ae_WeightTbl_16_31




	//=================================================================================================
	//	Set GAS & CCM White Point
	//=================================================================================================
	// param_start	TVAR_ash_AwbAshCord
	{0x002A, 0x0704},
	{0x0F12, 0x0090},	//000E	// #TVAR_ash_AwbAshCord_0_
	{0x0F12, 0x00B3},	//0012	// #TVAR_ash_AwbAshCord_1_
	{0x0F12, 0x00C3},	//0019	// #TVAR_ash_AwbAshCord_2_
	{0x0F12, 0x00E5},	//0034	// #TVAR_ash_AwbAshCord_3_
	{0x0F12, 0x0116},	//0044	// #TVAR_ash_AwbAshCord_4_
	{0x0F12, 0x0198},	//0052	// #TVAR_ash_AwbAshCord_5_
	{0x0F12, 0x0200},	//0058	// #TVAR_ash_AwbAshCord_6_

	// param_start	wbt_AwbCcmCord
	{0x002A, 0x06F2},
	{0x0F12, 0x00B3},	//0016	// #SARR_AwbCcmCord_0_	Hor
	{0x0F12, 0x00E5},	//001E	// #SARR_AwbCcmCord_1_	IncaA
	{0x0F12, 0x0100},	//0024	// #SARR_AwbCcmCord_2_	WW
	{0x0F12, 0x0116},	//0038	// #SARR_AwbCcmCord_3_	CW
	{0x0F12, 0x0150},	//004C	// #SARR_AwbCcmCord_4_	D50
	{0x0F12, 0x0198},	//0058	// #SARR_AwbCcmCord_5_	D65

	// Set Target Brightness Control
	{0x002A, 0x103E},
	{0x0F12, 0x0000},	// #SARR_IllumType_0_
	{0x0F12, 0x0009},	// #SARR_IllumType_1_
	{0x0F12, 0x0018},	// #SARR_IllumType_2_
	{0x0F12, 0x0032},	// #SARR_IllumType_3_
	{0x0F12, 0x004A},	// #SARR_IllumType_4_
	{0x0F12, 0x0051},	// #SARR_IllumType_5_
	{0x0F12, 0x0056},	// #SARR_IllumType_6_
	{0x0F12, 0x010C},	// #SARe_2_R_IllumTypeF_0_
	{0x0F12, 0x010C},	// #SARe_3_R_IllumTypeF_1_
	{0x0F12, 0x0109},	// #SARe_4_R_IllumTypeF_2_
	{0x0F12, 0x0105},	// #SARe_5_R_IllumTypeF_3_
	{0x0F12, 0x0102},	// #SARe_6_R_IllumTypeF_4_
	{0x0F12, 0x00FB},	// #SARR_IllumTypeF_5_
	{0x0F12, 0x00F8},	// #SARR_IllumTypeF_6_

	// Indoor GAS Alpha
	{0x002A, 0x0712},
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[0]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[1]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[2]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[3]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[4]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[5]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[6]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[7]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[8]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[9]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[10]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[11]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[12]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[13]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[14]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[15]
	{0x0F12, 0x00E0},	// #TVAR_ash_GASAlpha[16]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[17]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[18]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[19]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[20]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[21]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[22]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[23]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[24]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[25]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[26]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[27]

	// Outdoor GAS Alpha
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_0_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_1_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_2_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_3_

	// GAS LUT Start Address
	{0x002A, 0x0754},
	{0x0F12, 0x2388},	//#TVAR_ash_pGAS
	{0x0F12, 0x7000},	//#TVAR_ash_pGAS

	// GAS LUT
	//							//1030	//1027
	{0x002A, 0x2388},
	{0x0F12, 0x01C9},	//#TVAR_ash_pGAS[0]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[1]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[2]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[3]
	{0x0F12, 0x0105},	//#TVAR_ash_pGAS[4]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[5]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[6]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[7]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[8]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[9]
	{0x0F12, 0x0160},	//#TVAR_ash_pGAS[10]
	{0x0F12, 0x0189},	//#TVAR_ash_pGAS[11]
	{0x0F12, 0x01D0},	//#TVAR_ash_pGAS[12]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[13]
	{0x0F12, 0x016A},	//#TVAR_ash_pGAS[14]
	{0x0F12, 0x0126},	//#TVAR_ash_pGAS[15]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[16]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[17]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[18]
	{0x0F12, 0x009B},	//#TVAR_ash_pGAS[19]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[20]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[21]
	{0x0F12, 0x00EF},	//#TVAR_ash_pGAS[22]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[23]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[24]
	{0x0F12, 0x019C},	//#TVAR_ash_pGAS[25]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[26]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[27]
	{0x0F12, 0x00E2},	//#TVAR_ash_pGAS[28]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[29]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[30]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[31]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[32]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[33]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[34]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[35]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[36]
	{0x0F12, 0x0133},	//#TVAR_ash_pGAS[37]
	{0x0F12, 0x016E},	//#TVAR_ash_pGAS[38]
	{0x0F12, 0x0140},	//#TVAR_ash_pGAS[39]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[40]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[41]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[42]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[43]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[44]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[45]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[46]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[47]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[48]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[49]
	{0x0F12, 0x010D},	//#TVAR_ash_pGAS[50]
	{0x0F12, 0x0157},	//#TVAR_ash_pGAS[51]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[52]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[53]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[54]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[55]
	{0x0F12, 0x002F},	//#TVAR_ash_pGAS[56]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[57]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[58]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[59]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[60]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[61]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[62]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[63]
	{0x0F12, 0x0142},	//#TVAR_ash_pGAS[64]
	{0x0F12, 0x011A},	//#TVAR_ash_pGAS[65]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[66]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[67]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[68]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[69]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[70]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[71]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[72]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[73]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[74]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[75]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[76]
	{0x0F12, 0x013B},	//#TVAR_ash_pGAS[77]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[78]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[79]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[80]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[81]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[82]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[83]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[84]
	{0x0F12, 0x0014},	//#TVAR_ash_pGAS[85]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[86]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[87]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[88]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[89]
	{0x0F12, 0x013F},	//#TVAR_ash_pGAS[90]
	{0x0F12, 0x012C},	//#TVAR_ash_pGAS[91]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[92]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[93]
	{0x0F12, 0x0067},	//#TVAR_ash_pGAS[94]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[95]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[96]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[97]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[98]
	{0x0F12, 0x004B},	//#TVAR_ash_pGAS[99]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[100]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[101]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[102]
	{0x0F12, 0x0150},	//#TVAR_ash_pGAS[103]
	{0x0F12, 0x0149},	//#TVAR_ash_pGAS[104]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[105]
	{0x0F12, 0x00C5},	//#TVAR_ash_pGAS[106]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[107]
	{0x0F12, 0x0061},	//#TVAR_ash_pGAS[108]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[109]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[110]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[111]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[112]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[113]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[114]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[115]
	{0x0F12, 0x016D},	//#TVAR_ash_pGAS[116]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[117]
	{0x0F12, 0x013A},	//#TVAR_ash_pGAS[118]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[119]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[120]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[121]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[122]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[123]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[124]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[125]
	{0x0F12, 0x00D8},	//#TVAR_ash_pGAS[126]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[127]
	{0x0F12, 0x0154},	//#TVAR_ash_pGAS[128]
	{0x0F12, 0x0186},	//#TVAR_ash_pGAS[129]
	{0x0F12, 0x018F},	//#TVAR_ash_pGAS[130]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[131]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[132]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[133]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[134]
	{0x0F12, 0x00B4},	//#TVAR_ash_pGAS[135]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[136]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[137]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[138]
	{0x0F12, 0x0112},	//#TVAR_ash_pGAS[139]
	{0x0F12, 0x014F},	//#TVAR_ash_pGAS[140]
	{0x0F12, 0x0185},	//#TVAR_ash_pGAS[141]
	{0x0F12, 0x01A1},	//#TVAR_ash_pGAS[142]
	{0x0F12, 0x0168},	//#TVAR_ash_pGAS[143]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[144]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[145]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[146]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[147]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[148]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[149]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[150]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[151]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[152]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[153]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[154]
	{0x0F12, 0x0163},	//#TVAR_ash_pGAS[155]
	{0x0F12, 0x0131},	//#TVAR_ash_pGAS[156]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[157]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[158]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[159]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[160]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[161]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[162]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[163]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[164]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[165]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[166]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[167]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[168]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[169]
	{0x0F12, 0x00D2},	//#TVAR_ash_pGAS[170]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[171]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[172]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[173]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[174]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[175]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[176]
	{0x0F12, 0x0059},	//#TVAR_ash_pGAS[177]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[178]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[179]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[180]
	{0x0F12, 0x010C},	//#TVAR_ash_pGAS[181]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[182]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[183]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[184]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[185]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[186]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[187]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[188]
	{0x0F12, 0x0025},	//#TVAR_ash_pGAS[189]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[190]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[191]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[192]
	{0x0F12, 0x00C2},	//#TVAR_ash_pGAS[193]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[194]
	{0x0F12, 0x00DD},	//#TVAR_ash_pGAS[195]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[196]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[197]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[198]
	{0x0F12, 0x001E},	//#TVAR_ash_pGAS[199]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[200]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[201]
	{0x0F12, 0x0010},	//#TVAR_ash_pGAS[202]
	{0x0F12, 0x0026},	//#TVAR_ash_pGAS[203]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[204]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[205]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[206]
	{0x0F12, 0x00EC},	//#TVAR_ash_pGAS[207]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[208]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[209]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[210]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[211]
	{0x0F12, 0x0017},	//#TVAR_ash_pGAS[212]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[213]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[214]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[215]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[216]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[217]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[218]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[219]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[220]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[221]
	{0x0F12, 0x009A},	//#TVAR_ash_pGAS[222]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[223]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[224]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[225]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[226]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[227]
	{0x0F12, 0x000F},	//#TVAR_ash_pGAS[228]
	{0x0F12, 0x0027},	//#TVAR_ash_pGAS[229]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[230]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[231]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[232]
	{0x0F12, 0x00F1},	//#TVAR_ash_pGAS[233]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[234]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[235]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[236]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[237]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[238]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[239]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[240]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[241]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[242]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[243]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[244]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[245]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[246]
	{0x0F12, 0x00FA},	//#TVAR_ash_pGAS[247]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[248]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[249]
	{0x0F12, 0x0064},	//#TVAR_ash_pGAS[250]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[251]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[252]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[253]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[254]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[255]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[256]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[257]
	{0x0F12, 0x00DF},	//#TVAR_ash_pGAS[258]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[259]
	{0x0F12, 0x0110},	//#TVAR_ash_pGAS[260]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[261]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[262]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[263]
	{0x0F12, 0x006E},	//#TVAR_ash_pGAS[264]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[265]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[266]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[267]
	{0x0F12, 0x0083},	//#TVAR_ash_pGAS[268]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[269]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[270]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[271]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[272]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[273]
	{0x0F12, 0x0104},	//#TVAR_ash_pGAS[274]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[275]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[276]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[277]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[278]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[279]
	{0x0F12, 0x0098},	//#TVAR_ash_pGAS[280]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[281]
	{0x0F12, 0x00D1},	//#TVAR_ash_pGAS[282]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[283]
	{0x0F12, 0x0127},	//#TVAR_ash_pGAS[284]
	{0x0F12, 0x0146},	//#TVAR_ash_pGAS[285]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[286]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[287]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[288]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[289]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[290]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[291]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[292]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[293]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[294]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[295]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[296]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[297]
	{0x0F12, 0x0180},	//#TVAR_ash_pGAS[298]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[299]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[300]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[301]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[302]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[303]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[304]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[305]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[306]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[307]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[308]
	{0x0F12, 0x00EE},	//#TVAR_ash_pGAS[309]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[310]
	{0x0F12, 0x014C},	//#TVAR_ash_pGAS[311]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[312]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[313]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[314]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[315]
	{0x0F12, 0x0057},	//#TVAR_ash_pGAS[316]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[317]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[318]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[319]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[320]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[321]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[322]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[323]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[324]
	{0x0F12, 0x00F6},	//#TVAR_ash_pGAS[325]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[326]
	{0x0F12, 0x0081},	//#TVAR_ash_pGAS[327]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[328]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[329]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[330]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[331]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[332]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[333]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[334]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[335]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[336]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[337]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[338]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[339]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[340]
	{0x0F12, 0x0041},	//#TVAR_ash_pGAS[341]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[342]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[343]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[344]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[345]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[346]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[347]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[348]
	{0x0F12, 0x00BA},	//#TVAR_ash_pGAS[349]
	{0x0F12, 0x00F7},	//#TVAR_ash_pGAS[350]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[351]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[352]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[353]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[354]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[355]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[356]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[357]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[358]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[359]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[360]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[361]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[362]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[363]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[364]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[365]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[366]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[367]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[368]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[369]
	{0x0F12, 0x0002},	//#TVAR_ash_pGAS[370]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[371]
	{0x0F12, 0x0023},	//#TVAR_ash_pGAS[372]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[373]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[374]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[375]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[376]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[377]
	{0x0F12, 0x00B0},	//#TVAR_ash_pGAS[378]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[379]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[380]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[381]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[382]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[383]
	{0x0F12, 0x001C},	//#TVAR_ash_pGAS[384]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[385]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[386]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[387]
	{0x0F12, 0x00B7},	//#TVAR_ash_pGAS[388]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[389]
	{0x0F12, 0x0106},	//#TVAR_ash_pGAS[390]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[391]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[392]
	{0x0F12, 0x006C},	//#TVAR_ash_pGAS[393]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[394]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[395]
	{0x0F12, 0x0033},	//#TVAR_ash_pGAS[396]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[397]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[398]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[399]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[400]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[401]
	{0x0F12, 0x0101},	//#TVAR_ash_pGAS[402]
	{0x0F12, 0x0120},	//#TVAR_ash_pGAS[403]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[404]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[405]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[406]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[407]
	{0x0F12, 0x0062},	//#TVAR_ash_pGAS[408]
	{0x0F12, 0x005C},	//#TVAR_ash_pGAS[409]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[410]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[411]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[412]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[413]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[414]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[415]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[416]
	{0x0F12, 0x0115},	//#TVAR_ash_pGAS[417]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[418]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[419]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[420]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[421]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[422]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[423]
	{0x0F12, 0x00A4},	//#TVAR_ash_pGAS[424]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[425]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[426]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[427]
	{0x0F12, 0x0130},	//#TVAR_ash_pGAS[428]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[429]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[430]
	{0x0F12, 0x00C6},	//#TVAR_ash_pGAS[431]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[432]
	{0x0F12, 0x0090},	//#TVAR_ash_pGAS[433]
	{0x0F12, 0x0085},	//#TVAR_ash_pGAS[434]
	{0x0F12, 0x0082},	//#TVAR_ash_pGAS[435]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[436]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[437]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[438]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[439]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[440]
	{0x0F12, 0x0135},	//#TVAR_ash_pGAS[441]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[442]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[443]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[444]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[445]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[446]
	{0x0F12, 0x005D},	//#TVAR_ash_pGAS[447]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[448]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[449]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[450]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[451]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[452]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[453]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[454]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[455]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[456]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[457]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[458]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[459]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[460]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[461]
	{0x0F12, 0x003D},	//#TVAR_ash_pGAS[462]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[463]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[464]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[465]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[466]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[467]
	{0x0F12, 0x00B5},	//#TVAR_ash_pGAS[468]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[469]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[470]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[471]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[472]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[473]
	{0x0F12, 0x001A},	//#TVAR_ash_pGAS[474]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[475]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[476]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[477]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[478]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[479]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[480]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[481]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[482]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[483]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[484]
	{0x0F12, 0x0018},	//#TVAR_ash_pGAS[485]
	{0x0F12, 0x000A},	//#TVAR_ash_pGAS[486]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[487]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[488]
	{0x0F12, 0x001F},	//#TVAR_ash_pGAS[489]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[490]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[491]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[492]
	{0x0F12, 0x00B6},	//#TVAR_ash_pGAS[493]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[494]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[495]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[496]
	{0x0F12, 0x0028},	//#TVAR_ash_pGAS[497]
	{0x0F12, 0x0012},	//#TVAR_ash_pGAS[498]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[499]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[500]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[501]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[502]
	{0x0F12, 0x0030},	//#TVAR_ash_pGAS[503]
	{0x0F12, 0x0050},	//#TVAR_ash_pGAS[504]
	{0x0F12, 0x007B},	//#TVAR_ash_pGAS[505]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[506]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[507]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[508]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[509]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[510]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[511]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[512]
	{0x0F12, 0x0001},	//#TVAR_ash_pGAS[513]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[514]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[515]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[516]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[517]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[518]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[519]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[520]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[521]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[522]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[523]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[524]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[525]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[526]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[527]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[528]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[529]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[530]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[531]
	{0x0F12, 0x00AE},	//#TVAR_ash_pGAS[532]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[533]
	{0x0F12, 0x0097},	//#TVAR_ash_pGAS[534]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[535]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[536]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[537]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[538]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[539]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[540]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[541]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[542]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[543]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[544]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[545]
	{0x0F12, 0x00D3},	//#TVAR_ash_pGAS[546]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[547]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[548]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[549]
	{0x0F12, 0x0058},	//#TVAR_ash_pGAS[550]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[551]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[552]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[553]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[554]
	{0x0F12, 0x006D},	//#TVAR_ash_pGAS[555]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[556]
	{0x0F12, 0x00AC},	//#TVAR_ash_pGAS[557]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[558]
	{0x0F12, 0x00F5},	//#TVAR_ash_pGAS[559]
	{0x0F12, 0x00D4},	//#TVAR_ash_pGAS[560]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[561]
	{0x0F12, 0x008E},	//#TVAR_ash_pGAS[562]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[563]
	{0x0F12, 0x0070},	//#TVAR_ash_pGAS[564]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[565]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[566]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[567]
	{0x0F12, 0x0093},	//#TVAR_ash_pGAS[568]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[569]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[570]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[571]



	//=================================================================================================
	//	Set Gamma LUT
	//=================================================================================================
	//							//New	//Hong	//ORG
	{0x002A, 0x04CC},
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[0][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[0][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[0][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[0][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[0][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[0][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[0][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[0][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[0][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[0][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[0][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[0][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[0][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[0][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[0][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[0][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[1][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[1][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[1][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[1][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[1][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[1][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[1][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[1][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[1][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[1][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[1][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[1][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[1][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[1][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[1][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[1][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[2][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[2][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[2][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[2][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[2][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[2][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[2][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[2][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[2][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[2][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[2][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[2][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[2][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[2][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[2][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[2][15]



	//=================================================================================================
	//	Set AWB
	//=================================================================================================
	{0x002A, 0x0DA8},
	{0x0F12, 0x0020},	// #awbb_LowBr0_NBzone	// 2008.04.24. eliminate to the purple color at low light.
	{0x002A, 0x0E8C},
	{0x0F12, 0x0000},	// #awbb_LowBr0_PatchNumZone
	{0x002A, 0x0D6C},
	{0x0F12, 0x0040},	// #awbb_YMedMoveToYAv

	// Indoor Gray Zone
	{0x002A, 0x0B9C},
	{0x0F12, 0x0339},	//0392	// #awbb_IndoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x0393},	//03A4	// #awbb_IndoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x031B},	//036F	// #awbb_IndoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x039C},	//03AE	// #awbb_IndoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x030A},	//034E	// #awbb_IndoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0398},	//03A2	// #awbb_IndoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x02F3},	//0331	// #awbb_IndoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0392},	//0392	// #awbb_IndoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x02E0},	//0315	// #awbb_IndoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0388},	//0381	// #awbb_IndoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x02CB},	//02F8	// #awbb_IndoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0374},	//036C	// #awbb_IndoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x02BB},	//02E1	// #awbb_IndoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x034B},	//0358	// #awbb_IndoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x02AA},	//02C5	// #awbb_IndoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x0336},	//033B	// #awbb_IndoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x029C},	//02B1	// #awbb_IndoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0321},	//0323	// #awbb_IndoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x028B},	//029C	// #awbb_IndoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x030B},	//030F	// #awbb_IndoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x027B},	//028B	// #awbb_IndoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02FD},	//02FA	// #awbb_IndoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x0268},	//027C	// #awbb_IndoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x02EA},	//02E2	// #awbb_IndoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x0259},	//0268	// #awbb_IndoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x02D8},	//02D1	// #awbb_IndoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x024C},	//0259	// #awbb_IndoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x02CB},	//02BF	// #awbb_IndoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x023D},	//024C	// #awbb_IndoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x02BD},	//02B1	// #awbb_IndoorGrZones_m_BGrid_14__m_right
	{0x0F12, 0x0231},	//023D	// #awbb_IndoorGrZones_m_BGrid_15__m_left
	{0x0F12, 0x02B1},	//02A2	// #awbb_IndoorGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0222},	//0231	// #awbb_IndoorGrZones_m_BGrid_16__m_left
	{0x0F12, 0x02A8},	//0299	// #awbb_IndoorGrZones_m_BGrid_16__m_right
	{0x0F12, 0x0215},	//0222	// #awbb_IndoorGrZones_m_BGrid_17__m_left
	{0x0F12, 0x029B},	//0290	// #awbb_IndoorGrZones_m_BGrid_17__m_right
	{0x0F12, 0x0208},	//0215	// #awbb_IndoorGrZones_m_BGrid_18__m_left
	{0x0F12, 0x0290},	//0288	// #awbb_IndoorGrZones_m_BGrid_18__m_right
	{0x0F12, 0x01F7},	//0208	// #awbb_IndoorGrZones_m_BGrid_19__m_left
	{0x0F12, 0x0287},	//0281	// #awbb_IndoorGrZones_m_BGrid_19__m_right
	{0x0F12, 0x01E6},	//01F7	// #awbb_IndoorGrZones_m_BGrid_20__m_left
	{0x0F12, 0x027F},	//0279	// #awbb_IndoorGrZones_m_BGrid_20__m_right
	{0x0F12, 0x01D9},	//01E6	// #awbb_IndoorGrZones_m_BGrid_21__m_left
	{0x0F12, 0x0279},	//026E	// #awbb_IndoorGrZones_m_BGrid_21__m_right
	{0x0F12, 0x01CC},	//01D9	// #awbb_IndoorGrZones_m_BGrid_22__m_left
	{0x0F12, 0x0272},	//0268	// #awbb_IndoorGrZones_m_BGrid_22__m_right
	{0x0F12, 0x01C1},	//01CC	// #awbb_IndoorGrZones_m_BGrid_23__m_left
	{0x0F12, 0x026C},	//0263	// #awbb_IndoorGrZones_m_BGrid_23__m_right
	{0x0F12, 0x01B3},	//01C1	// #awbb_IndoorGrZones_m_BGrid_24__m_left
	{0x0F12, 0x0265},	//0260	// #awbb_IndoorGrZones_m_BGrid_24__m_right
	{0x0F12, 0x01AA},	//01B3	// #awbb_IndoorGrZones_m_BGrid_25__m_left
	{0x0F12, 0x025C},	//0259	// #awbb_IndoorGrZones_m_BGrid_25__m_right
	{0x0F12, 0x019F},	//01AA	// #awbb_IndoorGrZones_m_BGrid_26__m_left
	{0x0F12, 0x0250},	//0253	// #awbb_IndoorGrZones_m_BGrid_26__m_right
	{0x0F12, 0x019B},	//019F	// #awbb_IndoorGrZones_m_BGrid_27__m_left
	{0x0F12, 0x0247},	//0248	// #awbb_IndoorGrZones_m_BGrid_27__m_right
	{0x0F12, 0x0197},	//019B	// #awbb_IndoorGrZones_m_BGrid_28__m_left
	{0x0F12, 0x0239},	//023F	// #awbb_IndoorGrZones_m_BGrid_28__m_right
	{0x0F12, 0x019B},	//0197	// #awbb_IndoorGrZones_m_BGrid_29__m_left
	{0x0F12, 0x021D},	//0239	// #awbb_IndoorGrZones_m_BGrid_29__m_right
	{0x0F12, 0x019D},	//019B	// #awbb_IndoorGrZones_m_BGrid_30__m_left
	{0x0F12, 0x0202},	//021D	// #awbb_IndoorGrZones_m_BGrid_30__m_right
	{0x0F12, 0x01AC},	//019D	// #awbb_IndoorGrZones_m_BGrid_31__m_left
	{0x0F12, 0x01D3},	//020A	// #awbb_IndoorGrZones_m_BGrid_31__m_right
	{0x0F12, 0x0000},	//01AC	// #awbb_IndoorGrZones_m_BGrid_32__m_left
	{0x0F12, 0x0000},	//01D3	// #awbb_IndoorGrZones_m_BGrid_32__m_right
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_left
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_right

	{0x002A, 0x0C3C},
	{0x0F12, 0x0004},	// #awbb_IndoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0138},	// #awbb_IndoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x0F12, 0x0000},

	// Outdoor Gray Zone
	{0x0F12, 0x0255},	//#awbb_OutdoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x0236},	//#awbb_OutdoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x0290},	//#awbb_OutdoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x0227},	//#awbb_OutdoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0297},	//#awbb_OutdoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x0210},	//#awbb_OutdoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0293},	//#awbb_OutdoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0200},	//#awbb_OutdoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x028B},	//#awbb_OutdoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x01F1},	//#awbb_OutdoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0286},	//#awbb_OutdoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x0282},	//#awbb_OutdoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x01E0},	//#awbb_OutdoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x027B},	//#awbb_OutdoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x01DE},	//#awbb_OutdoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0274},	//#awbb_OutdoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01DF},	//#awbb_OutdoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x0263},	//#awbb_OutdoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01F6},	//#awbb_OutdoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x0253},	//#awbb_OutdoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x020B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x023B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_right

	{0x002A, 0x0CB8},
	{0x0F12, 0x0004},	// #awbb_OutdoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x000C},	//#awbb_OutdoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0217},	//#awbb_OutdoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high
	{0x0F12, 0x0000},

	// Low Brightness Gray Zone
	{0x0F12, 0x0324},	//031E  // #awbb_LowBrGrZones_m_BGrid_0__m_left
	{0x0F12, 0x03A1},	//037E  // #awbb_LowBrGrZones_m_BGrid_0__m_right
	{0x0F12, 0x02F2},	//02CF  // #awbb_LowBrGrZones_m_BGrid_1__m_left
	{0x0F12, 0x03BA},	//0389  // #awbb_LowBrGrZones_m_BGrid_1__m_right
	{0x0F12, 0x02C4},	//02A6  // #awbb_LowBrGrZones_m_BGrid_2__m_left
	{0x0F12, 0x03B6},	//0386  // #awbb_LowBrGrZones_m_BGrid_2__m_right
	{0x0F12, 0x029E},	//0282  // #awbb_LowBrGrZones_m_BGrid_3__m_left
	{0x0F12, 0x03A6},	//0374  // #awbb_LowBrGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0283},	//0259  // #awbb_LowBrGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0380},	//0352  // #awbb_LowBrGrZones_m_BGrid_4__m_right
	{0x0F12, 0x025F},	//0239  // #awbb_LowBrGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0344},	//0330  // #awbb_LowBrGrZones_m_BGrid_5__m_right
	{0x0F12, 0x0241},	//021E  // #awbb_LowBrGrZones_m_BGrid_6__m_left
	{0x0F12, 0x031F},	//031B  // #awbb_LowBrGrZones_m_BGrid_6__m_right
	{0x0F12, 0x0227},	//020B  // #awbb_LowBrGrZones_m_BGrid_7__m_left
	{0x0F12, 0x02FC},	//0300  // #awbb_LowBrGrZones_m_BGrid_7__m_right
	{0x0F12, 0x020A},	//01F1  // #awbb_LowBrGrZones_m_BGrid_8__m_left
	{0x0F12, 0x02D9},	//02DD  // #awbb_LowBrGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01F1},	//01E0  // #awbb_LowBrGrZones_m_BGrid_9__m_left
	{0x0F12, 0x02C4},	//02C4  // #awbb_LowBrGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01D0},	//01CD  // #awbb_LowBrGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02B2},	//02AF  // #awbb_LowBrGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01B8},	//019E  // #awbb_LowBrGrZones_m_BGrid_11__m_left
	{0x0F12, 0x029E},	//02A0  // #awbb_LowBrGrZones_m_BGrid_11__m_right
	{0x0F12, 0x019F},	//0187  // #awbb_LowBrGrZones_m_BGrid_12__m_left
	{0x0F12, 0x0290},	//028C  // #awbb_LowBrGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0189},	//017D  // #awbb_LowBrGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0282},	//027E  // #awbb_LowBrGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0182},	//0179  // #awbb_LowBrGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0271},	//0258  // #awbb_LowBrGrZones_m_BGrid_14__m_right
	{0x0F12, 0x017F},	//0186  // #awbb_LowBrGrZones_m_BGrid_15__m_left
	{0x0F12, 0x0254},	//0232  // #awbb_LowBrGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0183},	//01BD  // #awbb_LowBrGrZones_m_BGrid_16__m_left
	{0x0F12, 0x022D},	//01FB  // #awbb_LowBrGrZones_m_BGrid_16__m_right
	{0x0F12, 0x019C},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_left
	{0x0F12, 0x01D3},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_right

	{0x002A, 0x0D34},
	{0x0F12, 0x0005},	// #awbb_LowBrGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0012},	// #awbb_LowBrGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0120},	// #awbb_LowBrGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0002},	// #awbb_LowBrGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D48},
	{0x0F12, 0x00E0},	// #awbb_LowBrGrZones_y_high
	{0x0F12, 0x0000},

	// Lowtemp Circle
	{0x0F12, 0x032F},	// #awbb_CrclLowT_R_c
	{0x0F12, 0x0000},
	{0x0F12, 0x017A},	// #awbb_CrclLowT_B_c
	{0x0F12, 0x0000},
	{0x0F12, 0x7300},	// #awbb_CrclLowT_Rad_c
	{0x0F12, 0x0000},
	{0x0F12, 0x000A},	// #awbb_CrclLowT_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D60},
	{0x0F12, 0x00E0},	// #awbb_CrclLowT_y_high
	{0x0F12, 0x0000},
	{0x002A, 0x0D82},
	{0x0F12, 0x0001},	// #awbb_ByPass_LowTempMode

	// White Locus
	{0x002A, 0x0D66},
	{0x0F12, 0x011F},	// #awbb_IntcR
	{0x0F12, 0x0132},	// #awbb_IntcB
	{0x002A, 0x0D74},
	{0x0F12, 0x052A},	// #awbb_MvEq_RBthresh

	// Gamut Thresholds
	{0x002A, 0x0DAE},
	{0x0F12, 0x0036},	// #awbb_GamutWidthThr2
	{0x0F12, 0x001C},	// #awbb_GamutHeightThr2
	{0x002A, 0x0DAA},
	{0x0F12, 0x071A},	// #awbb_GamutWidthThr1
	{0x0F12, 0x03A4},	// #awbb_GamutHeightThr1

	// SceneDetection Thresholds
	{0x002A, 0x0D92},
	{0x0F12, 0x0CE4},	//0A8C	// #awbb_SunnyBr
	{0x0F12, 0x0096},	// #awbb_Sunny_NBzone
	{0x002A, 0x0E86},
	{0x0F12, 0x0216},	// #awbb_OutdoorWP_r
	{0x0F12, 0x029F},	// #awbb_OutdoorWP_b
	{0x002A, 0x0D96},
	{0x0F12, 0x0580},	// #awbb_CloudyBr
	{0x0F12, 0x0096},	// #awbb_Cloudy_NBzone
	{0x002A, 0x0DB2},
	{0x0F12, 0x00DA},	// #awbb_CloudyRB
	{0x002A, 0x0D9A},
	{0x0F12, 0x000A},	// #awbb_Cloudy_BdivRzone
	{0x002A, 0x0DB4},
	{0x0F12, 0x0459},	// #awbb_LowTempRB
	{0x002A, 0x0DA4},
	{0x0F12, 0x000E},	// #awbb_LowTemp_RBzone
	{0x002A, 0x0D64},
	{0x0F12, 0x0040},	// #awbb_DarkBr
	{0x002A, 0x0DA6},
	{0x0F12, 0x001E},	// #awbb_LowBr_NBzone
	{0x002A, 0x0D9C},
	{0x0F12, 0x001B},	// #awbb_MacbethGamut_WidthZone
	{0x0F12, 0x000E},	// #awbb_MacbethGamut_HeightZone
	{0x0F12, 0x0008},	// #awbb_MacbethGamut_WidthZone2
	{0x0F12, 0x0004},	// #awbb_MacbethGamut_HeightZone2

	// AWB Debug.(Outdoor Pink)
	{0x002A, 0x0E30},
	{0x0F12, 0x0000},	//	#awbb_OutdoorFltrSz (outdoor WB를 moving average filtering)

	// UseInvalidOutdoor option
	{0x002A, 0x0D88},
	{0x0F12, 0x0001},	// #awbb_Use_InvalidOutDoor

	// AWB input Y-Filter setting
	{0x002A, 0x0C48},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x002A, 0x0CC4},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high

	{0x002A, 0x0DC2},
	{0x0F12, 0x0030},	// #awbb_GnCurPntImmunity
	{0x0F12, 0x00C8},	// #awbb_GnFarPntImmunity
	{0x0F12, 0x012C},	// #awbb_GnCurPntLongJump
	{0x0F12, 0x0258},	// #awbb_GainsMaxMove
	{0x0F12, 0x0003},	// #awbb_GnMinMatchToJump

	// RB Gain Offset
	//WRITE 70000E32	FFE0	//#awbb_RGainOff
	//WRITE 70000E34	FFe0	//#awbb_BGainOff


	//=================================================================================================
	//	Set CCM
	//=================================================================================================
	// CCM Start Address
	{0x002A, 0x06D0},
	{0x0F12, 0x2800},   //#TVAR_wbt_pBaseCcmsAddr[0]
	{0x0F12, 0x7000},
	{0x0F12, 0x2824},   //#TVAR_wbt_pBaseCcmsAddr[1]
	{0x0F12, 0x7000},
	{0x0F12, 0x2848},   //#TVAR_wbt_pBaseCcmsAddr[2]
	{0x0F12, 0x7000},
	{0x0F12, 0x286C},   //#TVAR_wbt_pBaseCcmsAddr[3]
	{0x0F12, 0x7000},
	{0x0F12, 0x2890},   //#TVAR_wbt_pBaseCcmsAddr[4]
	{0x0F12, 0x7000},
	{0x0F12, 0x28B4},   //#TVAR_wbt_pBaseCcmsAddr[5]
	{0x0F12, 0x7000},
	{0x002A, 0x06EC},
	{0x0F12, 0x28D8},   //#TVAR_wbt_pOutdoorCcm
	{0x0F12, 0x7000},

	{0x002A, 0x2800},
	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	// Outdoor CCM
	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEA4},
	{0x0F12, 0x020F},
	{0x0F12, 0xFF8F},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},



	//=================================================================================================
	//	Set NB
	//=================================================================================================
	{0x002A, 0x07EA},
	{0x0F12, 0x0000},	//afit_bUseNoiseInd 0 : NB 1: Noise Index

	// param_start	SARR_uNormBrInDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrInDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrInDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrInDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrInDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrInDoor[4]

	// param_start	SARR_uNormBrOutDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrOutDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrOutDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrOutDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrOutDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrOutDoor[4]



	//=================================================================================================
	//	Set AFIT
	//=================================================================================================
	// AFIT Start Address
	{0x002A, 0x0814},
	{0x0F12, 0x082C},	//#TVAR_afit_pBaseVals
	{0x0F12, 0x7000},	//#TVAR_afit_pBaseVals

	// AFIT Table	//0223
	{0x002A, 0x082C},
	{0x0F12, 0x0010},	//BRIGHTNESS
	{0x0F12, 0xFFF0},	//CONTRAST
	{0x0F12, 0xFFD0},	//SATURATION
	{0x0F12, 0x0000},	//SHARP_BLUR
	{0x0F12, 0x0000},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0028},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                   bin: desparity low
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"                bin: desparity high
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x06FF},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x030A},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"               ]negati fine
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"                  ]low fine
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"                ]high fine
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"                   ]high low thres
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0005},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01AA},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0006},	//BRIGHTNESS
	{0x0F12, 0x0000},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0000},	//SHARP_BLUR
	{0x0F12, 0x0000},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x000E},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                  bin: desparity low
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x08B7},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0308},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01A0},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x000E},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0089},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0202},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0202},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0206},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x803C},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0006},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0205},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0000},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8050},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0006},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0000},	//UVDenoise_iYLowThresh
	{0x0F12, 0x0000},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0105},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0200},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x805A},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[0]
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[1]
	{0x0F12, 0x0800},	// # afit_pConstBaseVals[2]
	{0x0F12, 0x0600},	// # afit_pConstBaseVals[3]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[4]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[5]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[6]
	{0x0F12, 0x0300},	// # afit_pConstBaseVals[7]
	{0x0F12, 0x0002},	// # afit_pConstBaseVals[8]
	{0x0F12, 0x0400},	// # afit_pConstBaseVals[9]
	{0x0F12, 0x0106},	// # afit_pConstBaseVals[10]
	{0x0F12, 0x0005},	// # afit_pConstBaseVals[11]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[12]
	{0x0F12, 0x0003},	// # afit_pConstBaseVals[13]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[14]
	{0x0F12, 0xFFD6},	// # afit_pConstBaseVals[15]
	{0x0F12, 0x53C1},	// # afit_pConstBaseVals[16]
	{0x0F12, 0xE1FE},	// # afit_pConstBaseVals[17]
	{0x0F12, 0x0001},	// # afit_pConstBaseVals[18]

	{0x002A, 0x01EA},
	{0x0F12, 0x0000},	//#REG_TC_UserSharpBlur
	{0x002A, 0x0E32},
	{0x0F12, 0xFFF0},	//#awbb_RGainOff
	{0x0F12, 0x0000},	//#awbb_BGainOff

	// For fast AE
	{0x002A, 0x0FAC},
	{0x0F12, 0x0001},	// AE Gain output [0]    default : 0x10
	{0x0F12, 0x0008},	// AE Gain output [1]    default : 0x20
	{0x0F12, 0x0015},	// AE Gain output [2]    default : 0x40

	// For fast AWB
	{0x002A, 0x0DCA},
	{0x0F12, 0x0001},	// awbb_GnMinMatchToJump = 0x02 (default:0x03)
	{0x0028, 0x7000},
	{0x002A, 0x0E98},
	{0x0F12, 0x0300},	//bp_uMaxBrightnessFactor 0180 -> 0300
	{0x002A, 0x0E9E},
	{0x0F12, 0x0300},	//bp_uMinBrightnessFactor 0180 -> 0300

	// Update Changed Registers
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},

	{0x0028, 0xD000},
	{0x002A, 0x1088},
	{0x0F12, 0x05D5},

	{0x0028, 0xD000},
	{0x002A, 0x1094},
	{0x0F12, 0x0010},

	{0x0028, 0xD000},
	{0x002A, 0x1082},
	{0x0F12, 0x03FF},

	{0x0028, 0xD000},
	{0x002A, 0x1084},
	{0x0F12, 0x03FF},

    {SEQUENCE_WAIT_MS, 0x0064},
    {SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_init_1280x720_30fps[] =
{
	{0x0028, 0xD000},
	{0x002A, 0x1030},
	{0x0F12, 0x0000},
	{0x002A, 0x0014},
	{0x0F12, 0x0001},

	{0xFFFE, 0x0014},// Delay 100 -> 20

	{0x0028, 0x7000},
	{0x002A, 0x2080},
	{0x0F12, 0xB510},	// 70002080               0
	{0x0F12, 0xF000},	// 70002082               2
	{0x0F12, 0xF8FC},	// 70002084               4
	{0x0F12, 0xBC10},	// 70002086               6
	{0x0F12, 0xBC08},	// 70002088               8
	{0x0F12, 0x4718},	// 7000208A               A
	{0x0F12, 0xB5FE},	// 7000208C               C
	{0x0F12, 0x0007},	// 7000208E               E
	{0x0F12, 0x683C},	// 70002090               0
	{0x0F12, 0x687E},	// 70002092               2
	{0x0F12, 0x1DA5},	// 70002094               4
	{0x0F12, 0x88A0},	// 70002096               6
	{0x0F12, 0x2800},	// 70002098               8
	{0x0F12, 0xD00B},	// 7000209A               A
	{0x0F12, 0x88A8},	// 7000209C               C
	{0x0F12, 0x2800},	// 7000209E               E
	{0x0F12, 0xD008},	// 700020A0               0
	{0x0F12, 0x8820},	// 700020A2               2
	{0x0F12, 0x8829},	// 700020A4               4
	{0x0F12, 0x4288},	// 700020A6               6
	{0x0F12, 0xD301},	// 700020A8               8
	{0x0F12, 0x1A40},	// 700020AA               A
	{0x0F12, 0xE000},	// 700020AC               C
	{0x0F12, 0x1A08},	// 700020AE               E
	{0x0F12, 0x9001},	// 700020B0               0
	{0x0F12, 0xE001},	// 700020B2               2
	{0x0F12, 0x2019},	// 700020B4               4
	{0x0F12, 0x9001},	// 700020B6               6
	{0x0F12, 0x497B},	// 700020B8               8
	{0x0F12, 0x466B},	// 700020BA               A
	{0x0F12, 0x8A48},	// 700020BC               C
	{0x0F12, 0x8118},	// 700020BE               E
	{0x0F12, 0x8A88},	// 700020C0               0
	{0x0F12, 0x8158},	// 700020C2               2
	{0x0F12, 0x4879},	// 700020C4               4
	{0x0F12, 0x8940},	// 700020C6               6
	{0x0F12, 0x0040},	// 700020C8               8
	{0x0F12, 0x2103},	// 700020CA               A
	{0x0F12, 0xF000},	// 700020CC               C
	{0x0F12, 0xF902},	// 700020CE               E
	{0x0F12, 0x88A1},	// 700020D0               0
	{0x0F12, 0x4288},	// 700020D2               2
	{0x0F12, 0xD908},	// 700020D4               4
	{0x0F12, 0x8828},	// 700020D6               6
	{0x0F12, 0x8030},	// 700020D8               8
	{0x0F12, 0x8868},	// 700020DA               A
	{0x0F12, 0x8070},	// 700020DC               C
	{0x0F12, 0x88A8},	// 700020DE               E
	{0x0F12, 0x6038},	// 700020E0               0
	{0x0F12, 0xBCFE},	// 700020E2               2
	{0x0F12, 0xBC08},	// 700020E4               4
	{0x0F12, 0x4718},	// 700020E6               6
	{0x0F12, 0x88A9},	// 700020E8               8
	{0x0F12, 0x4288},	// 700020EA               A
	{0x0F12, 0xD906},	// 700020EC               C
	{0x0F12, 0x8820},	// 700020EE               E
	{0x0F12, 0x8030},	// 700020F0               0
	{0x0F12, 0x8860},	// 700020F2               2
	{0x0F12, 0x8070},	// 700020F4               4
	{0x0F12, 0x88A0},	// 700020F6               6
	{0x0F12, 0x6038},	// 700020F8               8
	{0x0F12, 0xE7F2},	// 700020FA               A
	{0x0F12, 0x9801},	// 700020FC               C
	{0x0F12, 0xA902},	// 700020FE               E
	{0x0F12, 0xF000},	// 70002100               0
	{0x0F12, 0xF8EE},	// 70002102               2
	{0x0F12, 0x0033},	// 70002104               4
	{0x0F12, 0x0029},	// 70002106               6
	{0x0F12, 0x9A02},	// 70002108               8
	{0x0F12, 0x0020},	// 7000210A               A
	{0x0F12, 0xF000},	// 7000210C               C
	{0x0F12, 0xF8F0},	// 7000210E               E
	{0x0F12, 0x6038},	// 70002110               0
	{0x0F12, 0xE7E6},	// 70002112               2
	{0x0F12, 0xB5F0},	// 70002114               4
	{0x0F12, 0xB08B},	// 70002116               6
	{0x0F12, 0x0006},	// 70002118               8
	{0x0F12, 0x2000},	// 7000211A               A
	{0x0F12, 0x9004},	// 7000211C               C
	{0x0F12, 0x6835},	// 7000211E               E
	{0x0F12, 0x6874},	// 70002120               0
	{0x0F12, 0x68B0},	// 70002122               2
	{0x0F12, 0x900A},	// 70002124               4
	{0x0F12, 0x68F0},	// 70002126               6
	{0x0F12, 0x9009},	// 70002128               8
	{0x0F12, 0x4F60},	// 7000212A               A
	{0x0F12, 0x8979},	// 7000212C               C
	{0x0F12, 0x084A},	// 7000212E               E
	{0x0F12, 0x88A8},	// 70002130               0
	{0x0F12, 0x88A3},	// 70002132               2
	{0x0F12, 0x4298},	// 70002134               4
	{0x0F12, 0xD300},	// 70002136               6
	{0x0F12, 0x0018},	// 70002138               8
	{0x0F12, 0xF000},	// 7000213A               A
	{0x0F12, 0xF8E1},	// 7000213C               C
	{0x0F12, 0x9007},	// 7000213E               E
	{0x0F12, 0x0021},	// 70002140               0
	{0x0F12, 0x0028},	// 70002142               2
	{0x0F12, 0xAA04},	// 70002144               4
	{0x0F12, 0xF000},	// 70002146               6
	{0x0F12, 0xF8E3},	// 70002148               8
	{0x0F12, 0x9006},	// 7000214A               A
	{0x0F12, 0x88A8},	// 7000214C               C
	{0x0F12, 0x2800},	// 7000214E               E
	{0x0F12, 0xD102},	// 70002150               0
	{0x0F12, 0x27FF},	// 70002152               2
	{0x0F12, 0x1C7F},	// 70002154               4
	{0x0F12, 0xE047},	// 70002156               6
	{0x0F12, 0x88A0},	// 70002158               8
	{0x0F12, 0x2800},	// 7000215A               A
	{0x0F12, 0xD101},	// 7000215C               C
	{0x0F12, 0x2700},	// 7000215E               E
	{0x0F12, 0xE042},	// 70002160               0
	{0x0F12, 0x8820},	// 70002162               2
	{0x0F12, 0x466B},	// 70002164               4
	{0x0F12, 0x8198},	// 70002166               6
	{0x0F12, 0x8860},	// 70002168               8
	{0x0F12, 0x81D8},	// 7000216A               A
	{0x0F12, 0x8828},	// 7000216C               C
	{0x0F12, 0x8118},	// 7000216E               E
	{0x0F12, 0x8868},	// 70002170               0
	{0x0F12, 0x8158},	// 70002172               2
	{0x0F12, 0xA802},	// 70002174               4
	{0x0F12, 0xC803},	// 70002176               6
	{0x0F12, 0xF000},	// 70002178               8
	{0x0F12, 0xF8D2},	// 7000217A               A
	{0x0F12, 0x9008},	// 7000217C               C
	{0x0F12, 0x8ABA},	// 7000217E               E
	{0x0F12, 0x9808},	// 70002180               0
	{0x0F12, 0x466B},	// 70002182               2
	{0x0F12, 0x4342},	// 70002184               4
	{0x0F12, 0x9202},	// 70002186               6
	{0x0F12, 0x8820},	// 70002188               8
	{0x0F12, 0x8198},	// 7000218A               A
	{0x0F12, 0x8860},	// 7000218C               C
	{0x0F12, 0x81D8},	// 7000218E               E
	{0x0F12, 0x980A},	// 70002190               0
	{0x0F12, 0x9903},	// 70002192               2
	{0x0F12, 0xF000},	// 70002194               4
	{0x0F12, 0xF8C4},	// 70002196               6
	{0x0F12, 0x9A02},	// 70002198               8
	{0x0F12, 0x17D1},	// 7000219A               A
	{0x0F12, 0x0E09},	// 7000219C               C
	{0x0F12, 0x1889},	// 7000219E               E
	{0x0F12, 0x1209},	// 700021A0               0
	{0x0F12, 0x4288},	// 700021A2               2
	{0x0F12, 0xDD1F},	// 700021A4               4
	{0x0F12, 0x8820},	// 700021A6               6
	{0x0F12, 0x466B},	// 700021A8               8
	{0x0F12, 0x8198},	// 700021AA               A
	{0x0F12, 0x8860},	// 700021AC               C
	{0x0F12, 0x81D8},	// 700021AE               E
	{0x0F12, 0x980A},	// 700021B0               0
	{0x0F12, 0x9903},	// 700021B2               2
	{0x0F12, 0xF000},	// 700021B4               4
	{0x0F12, 0xF8B4},	// 700021B6               6
	{0x0F12, 0x9001},	// 700021B8               8
	{0x0F12, 0x8828},	// 700021BA               A
	{0x0F12, 0x466B},	// 700021BC               C
	{0x0F12, 0x8118},	// 700021BE               E
	{0x0F12, 0x8868},	// 700021C0               0
	{0x0F12, 0x8158},	// 700021C2               2
	{0x0F12, 0x980A},	// 700021C4               4
	{0x0F12, 0x9902},	// 700021C6               6
	{0x0F12, 0xF000},	// 700021C8               8
	{0x0F12, 0xF8AA},	// 700021CA               A
	{0x0F12, 0x8AB9},	// 700021CC               C
	{0x0F12, 0x9A08},	// 700021CE               E
	{0x0F12, 0x4351},	// 700021D0               0
	{0x0F12, 0x17CA},	// 700021D2               2
	{0x0F12, 0x0E12},	// 700021D4               4
	{0x0F12, 0x1851},	// 700021D6               6
	{0x0F12, 0x120A},	// 700021D8               8
	{0x0F12, 0x9901},	// 700021DA               A
	{0x0F12, 0xF000},	// 700021DC               C
	{0x0F12, 0xF890},	// 700021DE               E
	{0x0F12, 0x0407},	// 700021E0               0
	{0x0F12, 0x0C3F},	// 700021E2               2
	{0x0F12, 0xE000},	// 700021E4               4
	{0x0F12, 0x2700},	// 700021E6               6
	{0x0F12, 0x8820},	// 700021E8               8
	{0x0F12, 0x466B},	// 700021EA               A
	{0x0F12, 0xAA05},	// 700021EC               C
	{0x0F12, 0x8198},	// 700021EE               E
	{0x0F12, 0x8860},	// 700021F0               0
	{0x0F12, 0x81D8},	// 700021F2               2
	{0x0F12, 0x8828},	// 700021F4               4
	{0x0F12, 0x8118},	// 700021F6               6
	{0x0F12, 0x8868},	// 700021F8               8
	{0x0F12, 0x8158},	// 700021FA               A
	{0x0F12, 0xA802},	// 700021FC               C
	{0x0F12, 0xC803},	// 700021FE               E
	{0x0F12, 0x003B},	// 70002200               0
	{0x0F12, 0xF000},	// 70002202               2
	{0x0F12, 0xF895},	// 70002204               4
	{0x0F12, 0x88A1},	// 70002206               6
	{0x0F12, 0x88A8},	// 70002208               8
	{0x0F12, 0x003A},	// 7000220A               A
	{0x0F12, 0xF000},	// 7000220C               C
	{0x0F12, 0xF898},	// 7000220E               E
	{0x0F12, 0x0004},	// 70002210               0
	{0x0F12, 0xA804},	// 70002212               2
	{0x0F12, 0xC803},	// 70002214               4
	{0x0F12, 0x9A09},	// 70002216               6
	{0x0F12, 0x9B07},	// 70002218               8
	{0x0F12, 0xF000},	// 7000221A               A
	{0x0F12, 0xF889},	// 7000221C               C
	{0x0F12, 0xA806},	// 7000221E               E
	{0x0F12, 0xC805},	// 70002220               0
	{0x0F12, 0x0021},	// 70002222               2
	{0x0F12, 0xF000},	// 70002224               4
	{0x0F12, 0xF88C},	// 70002226               6
	{0x0F12, 0x6030},	// 70002228               8
	{0x0F12, 0xB00B},	// 7000222A               A
	{0x0F12, 0xBCF0},	// 7000222C               C
	{0x0F12, 0xBC08},	// 7000222E               E
	{0x0F12, 0x4718},	// 70002230               0
	{0x0F12, 0xB510},	// 70002232               2
	{0x0F12, 0x6800},	// 70002234               4
	{0x0F12, 0x2805},	// 70002236               6
	{0x0F12, 0xD11E},	// 70002238               8
	{0x0F12, 0x481D},	// 7000223A               A
	{0x0F12, 0x491D},	// 7000223C               C
	{0x0F12, 0x8800},	// 7000223E               E
	{0x0F12, 0x8348},	// 70002240               0
	{0x0F12, 0x2301},	// 70002242               2
	{0x0F12, 0x31A0},	// 70002244               4
	{0x0F12, 0x720B},	// 70002246               6
	{0x0F12, 0x2000},	// 70002248               8
	{0x0F12, 0x000A},	// 7000224A               A
	{0x0F12, 0x3A60},	// 7000224C               C
	{0x0F12, 0x7150},	// 7000224E               E
	{0x0F12, 0x71CB},	// 70002250               0
	{0x0F12, 0x22FF},	// 70002252               2
	{0x0F12, 0x3980},	// 70002254               4
	{0x0F12, 0x75CA},	// 70002256               6
	{0x0F12, 0x764A},	// 70002258               8
	{0x0F12, 0x000A},	// 7000225A               A
	{0x0F12, 0x3260},	// 7000225C               C
	{0x0F12, 0x7210},	// 7000225E               E
	{0x0F12, 0x7250},	// 70002260               0
	{0x0F12, 0x2008},	// 70002262               2
	{0x0F12, 0x7488},	// 70002264               4
	{0x0F12, 0x74C8},	// 70002266               6
	{0x0F12, 0x4813},	// 70002268               8
	{0x0F12, 0x217E},	// 7000226A               A
	{0x0F12, 0x43C9},	// 7000226C               C
	{0x0F12, 0x8101},	// 7000226E               E
	{0x0F12, 0x2306},	// 70002270               0
	{0x0F12, 0x5EC0},	// 70002272               2
	{0x0F12, 0xF000},	// 70002274               4
	{0x0F12, 0xF86C},	// 70002276               6
	{0x0F12, 0xBC10},	// 70002278               8
	{0x0F12, 0xBC08},	// 7000227A               A
	{0x0F12, 0x4718},	// 7000227C               C
	{0x0F12, 0xB570},	// 7000227E               E
	{0x0F12, 0x2200},	// 70002280               0
	{0x0F12, 0x490E},	// 70002282               2
	{0x0F12, 0x480E},	// 70002284               4
	{0x0F12, 0x2401},	// 70002286               6
	{0x0F12, 0xF000},	// 70002288               8
	{0x0F12, 0xF86A},	// 7000228A               A
	{0x0F12, 0x0022},	// 7000228C               C
	{0x0F12, 0x490D},	// 7000228E               E
	{0x0F12, 0x480D},	// 70002290               0
	{0x0F12, 0x2502},	// 70002292               2
	{0x0F12, 0xF000},	// 70002294               4
	{0x0F12, 0xF864},	// 70002296               6
	{0x0F12, 0x490C},	// 70002298               8
	{0x0F12, 0x480D},	// 7000229A               A
	{0x0F12, 0x002A},	// 7000229C               C
	{0x0F12, 0xF000},	// 7000229E               E
	{0x0F12, 0xF85F},	// 700022A0               0
	{0x0F12, 0xBC70},	// 700022A2               2
	{0x0F12, 0xBC08},	// 700022A4               4
	{0x0F12, 0x4718},	// 700022A6               6
	{0x0F12, 0x1A28},	// 700022A8               8
	{0x0F12, 0x7000},	// 700022AA               A
	{0x0F12, 0x0D64},	// 700022AC               C
	{0x0F12, 0x7000},	// 700022AE               E
	{0x0F12, 0x2370},	// 700022B0               0
	{0x0F12, 0x7000},	// 700022B2               2
	{0x0F12, 0x1C14},	// 700022B4               4
	{0x0F12, 0x7000},	// 700022B6               6
	{0x0F12, 0x14FA},	// 700022B8               8
	{0x0F12, 0x7000},	// 700022BA               A
	{0x0F12, 0x2115},	// 700022BC               C
	{0x0F12, 0x7000},	// 700022BE               E
	{0x0F12, 0x622F},	// 700022C0               0
	{0x0F12, 0x0000},	// 700022C2               2
	{0x0F12, 0x208D},	// 700022C4               4
	{0x0F12, 0x7000},	// 700022C6               6
	{0x0F12, 0x6445},	// 700022C8               8
	{0x0F12, 0x0000},	// 700022CA               A
	{0x0F12, 0x2233},	// 700022CC               C
	{0x0F12, 0x7000},	// 700022CE               E
	{0x0F12, 0x3B5B},	// 700022D0               0
	{0x0F12, 0x0000},	// 700022D2               2
	{0x0F12, 0x4778},	// 700022D4               4
	{0x0F12, 0x46C0},	// 700022D6               6
	{0x0F12, 0xF004},	// 700022D8               8
	{0x0F12, 0xE51F},	// 700022DA               A
	{0x0F12, 0xA464},	// 700022DC               C
	{0x0F12, 0x0000},	// 700022DE               E
	{0x0F12, 0x4778},	// 700022E0               0
	{0x0F12, 0x46C0},	// 700022E2               2
	{0x0F12, 0xC000},	// 700022E4               4
	{0x0F12, 0xE59F},	// 700022E6               6
	{0x0F12, 0xFF1C},	// 700022E8               8
	{0x0F12, 0xE12F},	// 700022EA               A
	{0x0F12, 0x6009},	// 700022EC               C
	{0x0F12, 0x0000},	// 700022EE               E
	{0x0F12, 0x4778},	// 700022F0               0
	{0x0F12, 0x46C0},	// 700022F2               2
	{0x0F12, 0xC000},	// 700022F4               4
	{0x0F12, 0xE59F},	// 700022F6               6
	{0x0F12, 0xFF1C},	// 700022F8               8
	{0x0F12, 0xE12F},	// 700022FA               A
	{0x0F12, 0x622F},	// 700022FC               C
	{0x0F12, 0x0000},	// 700022FE               E
	{0x0F12, 0x4778},	// 70002300               0
	{0x0F12, 0x46C0},	// 70002302               2
	{0x0F12, 0xC000},	// 70002304               4
	{0x0F12, 0xE59F},	// 70002306               6
	{0x0F12, 0xFF1C},	// 70002308               8
	{0x0F12, 0xE12F},	// 7000230A               A
	{0x0F12, 0x5F49},	// 7000230C               C
	{0x0F12, 0x0000},	// 7000230E               E
	{0x0F12, 0x4778},	// 70002310               0
	{0x0F12, 0x46C0},	// 70002312               2
	{0x0F12, 0xC000},	// 70002314               4
	{0x0F12, 0xE59F},	// 70002316               6
	{0x0F12, 0xFF1C},	// 70002318               8
	{0x0F12, 0xE12F},	// 7000231A               A
	{0x0F12, 0x5FC7},	// 7000231C               C
	{0x0F12, 0x0000},	// 7000231E               E
	{0x0F12, 0x4778},	// 70002320               0
	{0x0F12, 0x46C0},	// 70002322               2
	{0x0F12, 0xC000},	// 70002324               4
	{0x0F12, 0xE59F},	// 70002326               6
	{0x0F12, 0xFF1C},	// 70002328               8
	{0x0F12, 0xE12F},	// 7000232A               A
	{0x0F12, 0x5457},	// 7000232C               C
	{0x0F12, 0x0000},	// 7000232E               E
	{0x0F12, 0x4778},	// 70002330               0
	{0x0F12, 0x46C0},	// 70002332               2
	{0x0F12, 0xC000},	// 70002334               4
	{0x0F12, 0xE59F},	// 70002336               6
	{0x0F12, 0xFF1C},	// 70002338               8
	{0x0F12, 0xE12F},	// 7000233A               A
	{0x0F12, 0x5FA3},	// 7000233C               C
	{0x0F12, 0x0000},	// 7000233E               E
	{0x0F12, 0x4778},	// 70002340               0
	{0x0F12, 0x46C0},	// 70002342               2
	{0x0F12, 0xC000},	// 70002344               4
	{0x0F12, 0xE59F},	// 70002346               6
	{0x0F12, 0xFF1C},	// 70002348               8
	{0x0F12, 0xE12F},	// 7000234A               A
	{0x0F12, 0x51F9},	// 7000234C               C
	{0x0F12, 0x0000},	// 7000234E               E
	{0x0F12, 0x4778},	// 70002350               0
	{0x0F12, 0x46C0},	// 70002352               2
	{0x0F12, 0xC000},	// 70002354               4
	{0x0F12, 0xE59F},	// 70002356               6
	{0x0F12, 0xFF1C},	// 70002358               8
	{0x0F12, 0xE12F},	// 7000235A               A
	{0x0F12, 0x7633},	// 7000235C               C
	{0x0F12, 0x0000},	// 7000235E               E
	{0x0F12, 0x4778},	// 70002360               0
	{0x0F12, 0x46C0},	// 70002362               2
	{0x0F12, 0xC000},	// 70002364               4
	{0x0F12, 0xE59F},	// 70002366               6
	{0x0F12, 0xFF1C},	// 70002368               8
	{0x0F12, 0xE12F},	// 7000236A               A
	{0x0F12, 0xA007},	// 7000236C               C
	{0x0F12, 0x0000},	// 7000236E               E
	{0x0F12, 0x0360},	// 70002370               0
	{0x0F12, 0x6F4E},	// 70002372               2
	{0x0F12, 0x2076},	// 70002374               4
	{0x0F12, 0x3831},	// 70002376               6
	{0x0F12, 0x3220},	// 70002378               8
	{0x0F12, 0x3030},	// 7000237A               A
	{0x0F12, 0x0038},	// 7000237C               C
	{0x0F12, 0xE010},	// 7000237E               E
	{0x0F12, 0x0208},	// 70002380               0
	{0x0F12, 0x0038},	// 70002382               2
	// End of Trap and Patch (Last : 70002382h) // Total Size 772 (0x0304)
	{0x0028, 0xD000},
	{0x002A, 0x1000},
	{0x0F12, 0x0001},

	{0x0028, 0x7000},
	{0x002A, 0x0ADC},
	{0x0F12, 0x0AF0},	//#setot_uOnlineClocksDiv4OnlineC0             //2800
	{0x002A, 0x0AE2},
	{0x0F12, 0x222E},	//#setot_usSetRomWaitStatesSetRomThreshold4KHz //ROM Wait Threshold to 35MHz
	{0x002A, 0x0B94},
	{0x0F12, 0x0500},	//#awbb_GainsInit_0_:R    insInit
	{0x0F12, 0x0400},	//#awbb_GainsInit_1_:G    insInit
	{0x0F12, 0x0700},	//#awbb_GainsInit_2_:B    insInit
	{0x002A, 0x04A0},
	{0x0F12, 0x8000},	//#lt_uLeiInit:AE start   Init:AE
	{0x002A, 0x049A},
	{0x0F12, 0x003B},	//#lt_uMinExp             Exp



	//=================================================================================================
	//	Set CIS/APS/Analog
	//=================================================================================================
	// This registers are for FACTORY ONLY. If you change it without prior notification,
	// YOU are RESPONSIBLE for the FAILURE that will happen in the future.
	//=================================================================================================
	{0x0028, 0xD000},
	{0x002A, 0xF106},
	{0x0F12, 0x0001},	// L-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xF206},
	{0x0F12, 0x0001},	// F-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xC202},
	{0x0F12, 0x0700},	// tgr_coarse_integration_time[15:0]: revised by Ana 080115
	{0x002A, 0xF414},
	{0x0F12, 0x0030},	// aig_adc_sat[7:0] (aig_adc_sat[7:4]+1)*33mV + 690mV
	{0x002A, 0xC204},
	{0x0F12, 0x0100},	// tgr_analogue_code_global[12:0] Analog gain X8
	{0x002A, 0xF402},
	{0x0F12, 0x0092},	// aig_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_offset2[7:0]: revised by Ana 080425
	{0x002A, 0xF700},
	{0x0F12, 0x0040},	// bpradlc_control[7:0]: revised by Ana 080529 // bpradlc_nactive_pedestal[7:5],bpradlc_passthrough[1],bpradlc_bypass[0]
	{0x002A, 0xF708},
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_r[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gr[7:0]: revised by Tg 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gb[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_b[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_total[7:0]: revised by TG 080529
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_r[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gr[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gb[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_b[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_total[7:0]: revised by Ana 080425
	{0x0F12, 0x0001},	// bpradlc_adlc_en[7:0]: revised by TG 080529	// bpradlc_f_l_adlc_en[2],bpradlc_f_adlc_en[1],bpradlc_h_adlc_en[0]
	{0x0F12, 0x0015},	// bpradlc_adlc_option[7:0]: revised by TG 080425 // bpradlc_h_adlc_ch_sel[4], // bpradlc_max_data_clip_sel[3:2],bpradlc_f_adlc_max_data_clip_sel[1:0]
	{0x0F12, 0x0001},	// bpradlc_adlc_bpr_en[7:0] adlc_bpr_enable: revised by Ana 080112
	{0x0F12, 0x0040},	// bpradlc_adlc_bpr_thresh[7:0]: 080425
	{0x002A, 0xF48A},
	{0x0F12, 0x0048},	// aig_ld_ptr7[9:0] line OB start addr(04h~48h): revised by Ana 080911
	{0x002A, 0xF10A},
	{0x0F12, 0x008B},	// tgr_h_desc0_addr_end[10:0] line OB end addr(47h~8Bh): revised by Ana 080911 // line OB start - line OB end = 67d
	{0x002A, 0xF900},
	{0x0F12, 0x0067},	// cfpn_config: revised by Ana 080425 // cfpn_ref_gain[9:7],cpfn_ref_lines[6:5],cfpn_hbinning[4],cfpn_output_direct[3],// cfpn_overflow_protect[2],cpfn_passthrough[1],cfpn_bypass[0]
	{0x002A, 0xF406},
	{0x0F12, 0x0092},	// aig_cfpn_ref_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_cfpn_ref_offset2[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_cfpn_ref_gain[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_ld_ctrl[1:0] aig_ldb_en[1], aig_ld_en[0]
	{0x0F12, 0x0000},	// aig_row_id_ctrl[1:0]
	{0x002A, 0xF442},
	{0x0F12, 0x0000},	// aig_vavg[0]
	{0x0F12, 0x0000},	// aig_havg[0]=1b @H-avg mode: revised by Ana 080116
	{0x002A, 0xF448},
	{0x0F12, 0x0000},	// aig_sl_off[0]
	{0x002A, 0xF454},
	{0x0F12, 0x0001},	//Power save enable
	{0x0F12, 0x0001},	// aig_blst_en[0]
	{0x0F12, 0x0010},	// aig_blst_en_cintr[15:0]
	{0x0F12, 0x0000},	// aig_dshut_en[0]
	{0x002A, 0xF41A},
	{0x0F12, 0x00FF},	// aig_comp_bias[7:0] aig_comp2_bias[7:4], aig_comp1_bias[3:0]: revised by Ana 081013
	{0x0F12, 0x0003},	// aig_pix_bias[3:0]
	{0x002A, 0xF420},
	{0x0F12, 0x0030},	// aig_clp_lvl[7:0]: revised by Ana 080910(refer to 6AA)
	{0x002A, 0xF410},
	{0x0F12, 0x0001},	// aig_clp_sl_ctrl[0]
	{0x0F12, 0x0000},	// aig_cds_test[7:0], aig_cds_test[1]=1b @H-avg mode current save: revised by Ana 080116
	{0x002A, 0xF416},
	{0x0F12, 0x0001},	// aig_rmp_option[7:0]
	{0x002A, 0xF424},
	{0x0F12, 0x0000},	// aig_ref_option[7:0], aig_ref_option[0]=1b @H-avg mode odd cds off: revised by Ana 080116
	{0x002A, 0xF422},
	{0x0F12, 0x0000},	// aig_monit[7:0]
	{0x002A, 0xF41E},
	{0x0F12, 0x0000},	// aig_pd_pix[0]
	{0x002A, 0xF428},
	{0x0F12, 0x0000},	// aig_pd_cp[1:0] aig_pd_ncp[1], aig_pd_cp[0]
	{0x0F12, 0x0000},	// aig_pd_reg_pix[0]
	{0x0F12, 0x0000},	// aig_pd_reg_rg[0]
	{0x002A, 0xF430},
	{0x0F12, 0x0000},	// aig_pd_reg_tgsl[0]
	{0x0F12, 0x0000},	// aig_pd_reg_ntg[0]
	{0x0F12, 0x0008},	// aig_rosc_tune_cp[3:0]: revised by Ana 080418
	{0x0F12, 0x0005},	// aig_rosc_tune_ncp[3:0]
	{0x0F12, 0x000F},	// aig_cp_capa[3:0]
	{0x0F12, 0x0001},	// aig_reg_tune_pix[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_rg[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_tgsl[7:0]
	{0x0F12, 0x0010},	// aig_reg_tune_ntg[7:0] 0040 -> 0010
	{0x002A, 0xF4D6},
	{0x0F12, 0x00C0},	// aig_v15_tune[7:0]: revised by Ana 081010 // aig_v15_tune[7:4]=7h @sensor only Mode // aig_v15_tune[7:4]=9h @ISP 7.5fps Mode // aig_v15_tune[7:4]=Dh @ISP 15fps Mode
	{0x0F12, 0x0000},	//aig_vreg_sel[7:0] [9]h_test, [1]reg_sw_stby, [0]aig_reg_sel
	{0x002A, 0xF47C},
	{0x0F12, 0x000C},	//aig_ld_ptr0[4:0]
	{0x0F12, 0x0000},	//aig_ld_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF49A},
	{0x0F12, 0x0008},	//aig_sla_ptr0[3:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_sla_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4A2},
	{0x0F12, 0x0008},	//aig_slb_ptr0[7:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_slb_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4B2},
	{0x0F12, 0x0013},	//aig_rxa_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxa_ptr1[9:0]: revised by Ana 081010
	{0x0F12, 0x0013},	//aig_rxb_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxb_ptr1[9:0]: revised by Ana 081010
	{0x002A, 0xF4AA},
	{0x0F12, 0x009B},	//aig_txa_ptr0[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txa_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_txb_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txb_ptr1[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF474},
	{0x0F12, 0x0017},	//aig_s3_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x005F},	//aig_s3_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0017},	//aig_s4_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x008F},	//aig_s4_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF48C},
	{0x0F12, 0x0017},	//aig_clp_sl_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_clp_sl_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF4C8},
	{0x0F12, 0x0163},	//aig_off_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0193},	//aig_rmp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF490},
	{0x0F12, 0x0191},	//aig_comp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF418},
	{0x0F12, 0x0083},	//aig_dbs_option[7:0]: revised by Ana 081010

	//For ESD Check
	{0x0028, 0x7000},
	{0x002A, 0x0132},
	{0x0F12, 0xAAAA},	//#define REG_FWpid (For ESD Check)
	// * note : ESD Check Method.
	//	1. if (7000.0132 != AAAA ) reset // ( S/W reg. check)
	//	2. if (D000.1002 != 0000 ) reset // (internal Exception)

	//Set FPN Gain Input
	{0x002A, 0x1176},
	{0x0F12, 0x0020},	// fpn_GainInput[0]
	{0x0F12, 0x0040},	// fpn_GainInput[1]
	{0x0F12, 0x0080},	// fpn_GainInput[2]
	{0x0F12, 0x0100},	// fpn_GainInput[3]
	{0x0F12, 0x0014},	// fpn_GainOutput[0]
	{0x0F12, 0x000A},	// fpn_GainOutput[1]
	{0x0F12, 0x0008},	// fpn_GainOutput[2]
	{0x0F12, 0x0004},	// fpn_GainOutput[3]

	// Set CFPN Canceller
	{0x002A, 0x116E},
	{0x0F12, 0x0000},	// OvflProtect
	{0x0F12, 0x0000},	// bypassThrough
	{0x0F12, 0x0000},	// bypass
	{0x0F12, 0x0002},	// fpn.CollectMethod	// 0 : Center	1 : Right	2: LEFT
	{0x002A, 0x0AE8},
	{0x0F12, 0x0000},	// setot_bSendFpnToISP = ??

	// Set Forbidden Area
	{0x002A, 0x10EE},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_usRegCount // 0 value --> not use aig table 3
	{0x002A, 0x10F2},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_pSenModesRegsArray3[0]		//real register 0xD000F45A
	{0x002A, 0x1152},
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[48]    //real register 0xD000F4BA
	{0x0F12, 0x0028},	// senHal_SenRegsModes3_pSenModesRegsArray3[49]    //real register 0xD000F4BC
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[50]    //real register 0xD000F4BE
	{0x002A, 0x1148},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[43]    //real register 0xD000F4B0
	{0x002A, 0x1144},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[41]    //real register 0xD000F4AC
	{0x002A, 0x1150},
	{0x0F12, 0x0148},	// senHal_SenRegsModes3_pSenModesRegsArray3[47]    //real register 0xD000F4B8

	// Set AE Target
	{0x002A, 0x0F4C},
	{0x0F12, 0x003D},	// #TVAR_ae_BrAve



	//=================================================================================================
	//	Set Frame Rate
	//=================================================================================================
	{0x002A, 0x0484},
	{0x0F12, 0x8235},	// #lt_uMaxExp1 	// 66ms
	{0x0F12, 0x0000},
	{0x0F12, 0x86A0},	// #lt_uMaxExp2 	// 200ms
	{0x0F12, 0x0001},
	{0x0F12, 0x8214},	// #lt_uCapMaxExp1	// 66ms
	{0x0F12, 0x0000},
	{0x0F12, 0xD2F0},	// #lt_uCapMaxExp2	// 108ms
	{0x0F12, 0x0000},
	{0x0F12, 0x04C0},	// #lt_uMaxAnGain1	// x4.75
	{0x0F12, 0x0700},	// #lt_uMaxAnGain2	// x7.0
	{0x0F12, 0x0100},	// #lt_uMaxDigGain
	{0x002A, 0x0F52},
	{0x0F12, 0x0007},	// #ae_StatMode



	//=================================================================================================
	//	Set Flicker  Current Flicker = 7000.164A //Mon_AFC_usFlExpQunat		0001:Off 1045:60Hz	1388:50Hz
	//=================================================================================================

	//1. Auto Flicker 60Hz Start
	{0x002A, 0x0B2E},
	{0x0F12, 0x0001},	// #AFC_Default60Hz 	1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	{0x002A, 0x03F8},
	{0x0F12, 0x007F},	// #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////2. Auto Flicker 50Hz Start
	//WRITE	70000B2E	0000	// #AFC_Default60Hz		1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	//WRITE 700003F8	007F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////3. Manual Flicker 60Hz
	//WRITE 700003F8	005F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F
	//WRITE 700003D4	0002  // #REG_SF_USER_FlickerQuant	 //Flicker 50Hz:0001/60Hz:0002/off:0000
	//WRITE 700003D6	0001  // #REG_SF_USER_FlickerQuantChanged //if change 0001 write

	////4. Manual Flicker 50Hz
	//WRITE 700003F8	005F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F
	//WRITE 700003D4	0001  // #REG_SF_USER_FlickerQuant	 //Flicker 50Hz:0001/60Hz:0002/off:0000
	//WRITE 700003D6	0001  // #REG_SF_USER_FlickerQuantChanged //if change 0001 write

	////5. Flicker Off
	//WRITE 700003F8	005F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F
	//WRITE 700003D4	0000  // #REG_SF_USER_FlickerQuant	 //Flicker 50Hz:0001/60Hz:0002/off:0000
	//WRITE 700003D6	0001  // #REG_SF_USER_FlickerQuantChanged //if change 0001 write


	//WRITE 70000B36	0001  // #AFC_ManualQuant


	// added for test
	//WRITE #senHal_uMinColsAddAnalogBin       	 	0048		//Subsampling시 1-H min 값  1768로 조절하기 위한 Register
	//WRITE #setot_uOnlineClocksDiv40		 	  			0A28 		//2800
	//WRITE #setot_usSetRomWaitStateThreshold4KHz	222E 		//ROM Wait Threshold to 35MHz



	//=================================================================================================
	//	Set PLL
	//=================================================================================================
	// Input CLK
	{0x002A, 0x01B8},
	{0x0F12, 0x5DC0},	//5DC0	//#REG_TC_IPRM_InClockLSBs	//24 -> 24.5M Hz(20081208)
	{0x0F12, 0x0000},	//#REG_TC_IPRM_InClockMSBs

	// Target Sys CLK
	{0x002A, 0x01CC},
	{0x0F12, 0x1770},	// #REG_TC_IPRM_OpClk4KHz_0 			//24Mhz

	// Target PVI CLK
	{0x0F12, 0x444C},	// #REG_TC_IPRM_MinOutRate4KHz_0	//70Mhz
	{0x0F12, 0x446C},	// #REG_TC_IPRM_MaxOutRate4KHz_0


	// PLL ETC
	{0x002A, 0x01C6},
	{0x0F12, 0x0003},	//#REG_TC_IPRM_UseNPviClocks
	{0x0F12, 0x0000},	//#REG_TC_IPRM_UseNMipiClocks
	{0x002A, 0x01C0},
	{0x0F12, 0x0000},	//#EG_TC_IPRM_ValidVActiveLow
	{0x002A, 0x01DE},
	{0x0F12, 0x0001},	//#REG_TC_IPRM_UseRegsAPI
	{0x0F12, 0x0001},	//#REG_TC_IPRM_InitParamsUpdated
	{0x002A, 0x1084},
	{0x0F12, 0x0001},	//#senHal_bUseAnalogBinning //Avg S.S
	{0x002A, 0x10B6},
	{0x0F12, 0x0048},	//#senHal_uMinColsAddAnalogBin



	//=================================================================================================
	//	Set Preview Config
	//=================================================================================================

	//* Preview Cropping Code *//
	{0x002A,	0x020A},
	{0x0F12,	0x0640},	//#REG_TC_GP_PrevZoomReqInputWidth
	{0x0F12,	0x0384},	//#REG_TC_GP_PrevZoomReqInputHeight
	{0x0F12,	0x0000},	//#REG_TC_GP_PrevZoomReqInputWidthOfs
	{0x0F12,	0x0120},	//#REG_TC_GP_PrevZoomReqInputHeightOfs

	//* Zoom Update *//
	{0x002A,	0x021A},
	{0x0F12,	0x0001},	// #REG_TC_GP_Inputs_ChangeRequest : 0x021A

	{0x002A, 0x0242},
	{0x0F12, 0x0500}, //0x0280, //0x0140(320)->640, //090416 800 -> 320, 0x0320,	                 1280 -> 0x500  //0280  // #REG_0TC_PCFG_usWidth						// 640 -> 800 _20081209
	{0x0F12, 0x02D0}, //0x01e0, //0x00f0(240)->480, //090416 600 -> 240, 0x0258, 0x168 -> 360     720 -> 0x2D0	//01e0  // #REG_0TC_PCFG_usHeight						// 480 -> 600 _20081209
	{0x0F12, 0x0005},	// #REG_0TC_PCFG_Format 						// YUV output
//	{0x0F12, 0x2338},	// #REG_0TC_PCFG_usMaxOut4KHzRate
//	{0x0F12, 0x2318},	// #REG_0TC_PCFG_usMinOut4KHzRate
	{0x0F12, 0x446C},	// #REG_0TC_PCFG_usMaxOut4KHzRate
	{0x0F12, 0x444C},	// #REG_0TC_PCFG_usMinOut4KHzRate

	{0x0F12, 0x0050},	// #REG_0TC_PCFG_PVIMask   (x2: PCLK rising x0: PCLK Falling)
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_uClockInd
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_usFrTimeType
//	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType
	{0x0F12, 0x0002},	// #REG_0TC_PCFG_FrRateQualityType		(0 : bin mode, 2 : No-bin mode)
	{0x0F12, 0x03E8},	// #REG_0TC_PCFG_usMaxFrTimeMsecMult10 // 10fps
	{0x0F12, 0x029A},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 15fps

//	{0x0F12, 0x014D},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 30fps
	{0x002A, 0x0262},
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uPrevMirror -- X/Y flip 20090112
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uCaptureMirror -- X/Y flip 20090112
	//WRITE 70000262 000F  //#REG_0TC_PCFG_uPrevMirror
	//WRITE 70000264 000F  //#REG_0TC_PCFG_uCaptureMirror


	// Apply preview config
	{0x002A, 0x021C},
	{0x0F12, 0x0000},	//#REG_TC_GP_ActivePrevConfig	  //preview config0
	{0x002A, 0x0220},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevOpenAfterChange  //config 변경적용
	{0x002A, 0x01F8},
	{0x0F12, 0x0001},	//#REG_TC_GP_NewConfigSync
	{0x002A, 0x021E},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevConfigChanged



	//=================================================================================================
	//	Set Capture Config
	//=================================================================================================
	// For 1600x1202 capture
	//WRITE 70000202	0640	// #REG_TC_GP_CapReqInputWidth
	//WRITE 70000204	04C0	// #REG_TC_GP_CapReqInputHeight			// Vsize = 1216
	//WRITE 70000206	0000	// #REG_TC_GP_CapInputWidthOfs
	//WRITE 70000208	0000	// #REG_TC_GP_CapInputHeightOfs
	//
	//WRITE 70000212	0640	// #REG_TC_GP_CapZoomReqInputWidth
	//WRITE 70000214	04C0	// #REG_TC_GP_CapZoomReqInputHeight	// Vsize = 1216
	//WRITE 70000216	0000	// #REG_TC_GP_CapZoomReqInputWidthOfs
	//WRITE 70000218	0000	// #REG_TC_GP_CapZoomReqInputHeightOfs
	//WRITE 7000021A	0001	// #REG_TC_GP_InputsChangeRequest
	//WRITE 700001F8	0001	// #REG_TC_GP_NewConfigSync
	//

	{0x002A, 0x0302},
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uCaptureMode
	{0x0F12, 0x0640},	//#REG_0TC_CCFG_usWidth
	{0x0F12, 0x04B0},	//#REG_0TC_CCFG_usHeight						// Vsize = 1216
	{0x0F12, 0x0005},	//#REG_0TC_CCFG_Format	   //5:YUV,9:JPEG
//	{0x0F12, 0x2338},	//#REG_0TC_CCFG_usMaxOut4KHzRate
//	{0x0F12, 0x2318},	//#REG_0TC_CCFG_usMinOut4KHzRate
	{0x0F12, 0x446C},	//#REG_0TC_CCFG_usMaxOut4KHzRate
	{0x0F12, 0x444C},	//#REG_0TC_CCFG_usMinOut4KHzRate
	{0x0F12, 0x0050},	//#REG_0TC_CCFG_PVIMask (x2: PCLK rising	x0: PCLK Falling)
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uClockInd
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_usFrTimeType
	{0x0F12, 0x0002},	//#REG_0TC_CCFG_FrRateQualityType
	{0x0F12, 0x07D0},	//#REG_0TC_CCFG_usMaxFrTimeMsecMult10 // 5fps
	{0x0F12, 0x0535},	//#REG_0TC_CCFG_usMinFrTimeMsecMult10 // 7.5fps



	//=================================================================================================
	//	Set Periodic Mismatch (Use default value)
	//=================================================================================================
	//WRITE 70000780	0000 //FFF0  // #msm_uOffsetNoBin[0][0]
	//WRITE 70000782	0000 //FFF0  // #msm_uOffsetNoBin[0][1]
	//WRITE 70000784	0000 //FFEE  // #msm_uOffsetNoBin[1][0]
	//WRITE 70000786	0000 //FFE0  // #msm_uOffsetNoBin[1][1]
	//WRITE 70000788	0000 //FFF0  // #msm_uOffsetNoBin[2][0]
	//WRITE 7000078A	0000 //FFF0  // #msm_uOffsetNoBin[2][1]
	//WRITE 7000078C	0000 //FFEE  // #msm_uOffsetNoBin[3][0]
	//WRITE 7000078E	0000 //FFE0  // #msm_uOffsetNoBin[3][1]
	//WRITE 70000798  0000 //FFE0  // #msm_uOffsetBin[0][0]
	//WRITE 7000079A  0000 //FFEE  // #msm_uOffsetBin[0][1]
	//WRITE 7000079C  0000 //FFF0  // #msm_uOffsetBin[1][0]
	//WRITE 7000079E  0000 //FFFB  // #msm_uOffsetBin[1][1]



	//=================================================================================================
	//	Set AE Weights
	//=================================================================================================
	{0x002A, 0x0F5A},
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_0_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_1_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_2_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_3_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_4_
	{0x0F12, 0x0000},	//0201	// #ae_WeightTbl_16_5_
	{0x0F12, 0x0000},	//0102	// #ae_WeightTbl_16_6_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_7_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_8_
	{0x0F12, 0x0000},	//0202	// #ae_WeightTbl_16_9_
	{0x0F12, 0x0000},	//0202	// #ae_WeightTbl_16_10
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_11
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_12
	{0x0F12, 0x0101},	//0402	// #ae_WeightTbl_16_13
	{0x0F12, 0x0101},	//0204	// #ae_WeightTbl_16_14
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_15
	{0x0F12, 0x0201},	//0201	// #ae_WeightTbl_16_16
	{0x0F12, 0x0202},	//0504	// #ae_WeightTbl_16_17
	{0x0F12, 0x0102},	//0405	// #ae_WeightTbl_16_18
	{0x0F12, 0x0000},	//0102	// #ae_WeightTbl_16_19
	{0x0F12, 0x0201},	//0501	// #ae_WeightTbl_16_20
	{0x0F12, 0x0505},	//0505	// #ae_WeightTbl_16_21
	{0x0F12, 0x0102},	//0505	// #ae_WeightTbl_16_22
	{0x0F12, 0x0000},	//0105	// #ae_WeightTbl_16_23
	{0x0F12, 0x0201},	//0503	// #ae_WeightTbl_16_24
	{0x0F12, 0x0202},	//0505	// #ae_WeightTbl_16_25
	{0x0F12, 0x0102},	//0505	// #ae_WeightTbl_16_26
	{0x0F12, 0x0000},	//0305	// #ae_WeightTbl_16_27
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_28
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_29
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_30
	{0x0F12, 0x0000},	//0303	// #ae_WeightTbl_16_31




	//=================================================================================================
	//	Set GAS & CCM White Point
	//=================================================================================================
	// param_start	TVAR_ash_AwbAshCord
	{0x002A, 0x0704},
	{0x0F12, 0x0090},	//000E	// #TVAR_ash_AwbAshCord_0_
	{0x0F12, 0x00B3},	//0012	// #TVAR_ash_AwbAshCord_1_
	{0x0F12, 0x00C3},	//0019	// #TVAR_ash_AwbAshCord_2_
	{0x0F12, 0x00E5},	//0034	// #TVAR_ash_AwbAshCord_3_
	{0x0F12, 0x0116},	//0044	// #TVAR_ash_AwbAshCord_4_
	{0x0F12, 0x0198},	//0052	// #TVAR_ash_AwbAshCord_5_
	{0x0F12, 0x0200},	//0058	// #TVAR_ash_AwbAshCord_6_

	// param_start	wbt_AwbCcmCord
	{0x002A, 0x06F2},
	{0x0F12, 0x00B3},	//0016	// #SARR_AwbCcmCord_0_	Hor
	{0x0F12, 0x00E5},	//001E	// #SARR_AwbCcmCord_1_	IncaA
	{0x0F12, 0x0100},	//0024	// #SARR_AwbCcmCord_2_	WW
	{0x0F12, 0x0116},	//0038	// #SARR_AwbCcmCord_3_	CW
	{0x0F12, 0x0150},	//004C	// #SARR_AwbCcmCord_4_	D50
	{0x0F12, 0x0198},	//0058	// #SARR_AwbCcmCord_5_	D65

	// Set Target Brightness Control
	{0x002A, 0x103E},
	{0x0F12, 0x0000},	// #SARR_IllumType_0_
	{0x0F12, 0x0009},	// #SARR_IllumType_1_
	{0x0F12, 0x0018},	// #SARR_IllumType_2_
	{0x0F12, 0x0032},	// #SARR_IllumType_3_
	{0x0F12, 0x004A},	// #SARR_IllumType_4_
	{0x0F12, 0x0051},	// #SARR_IllumType_5_
	{0x0F12, 0x0056},	// #SARR_IllumType_6_
	{0x0F12, 0x010C},	// #SARe_2_R_IllumTypeF_0_
	{0x0F12, 0x010C},	// #SARe_3_R_IllumTypeF_1_
	{0x0F12, 0x0109},	// #SARe_4_R_IllumTypeF_2_
	{0x0F12, 0x0105},	// #SARe_5_R_IllumTypeF_3_
	{0x0F12, 0x0102},	// #SARe_6_R_IllumTypeF_4_
	{0x0F12, 0x00FB},	// #SARR_IllumTypeF_5_
	{0x0F12, 0x00F8},	// #SARR_IllumTypeF_6_

	// Indoor GAS Alpha
	{0x002A, 0x0712},
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[0]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[1]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[2]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[3]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[4]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[5]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[6]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[7]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[8]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[9]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[10]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[11]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[12]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[13]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[14]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[15]
	{0x0F12, 0x00E0},	// #TVAR_ash_GASAlpha[16]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[17]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[18]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[19]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[20]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[21]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[22]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[23]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[24]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[25]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[26]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[27]

	// Outdoor GAS Alpha
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_0_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_1_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_2_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_3_

	// GAS LUT Start Address
	{0x002A, 0x0754},
	{0x0F12, 0x2388},	//#TVAR_ash_pGAS
	{0x0F12, 0x7000},	//#TVAR_ash_pGAS

	// GAS LUT
	//							//1030	//1027
	{0x002A, 0x2388},
	{0x0F12, 0x01C9},	//#TVAR_ash_pGAS[0]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[1]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[2]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[3]
	{0x0F12, 0x0105},	//#TVAR_ash_pGAS[4]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[5]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[6]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[7]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[8]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[9]
	{0x0F12, 0x0160},	//#TVAR_ash_pGAS[10]
	{0x0F12, 0x0189},	//#TVAR_ash_pGAS[11]
	{0x0F12, 0x01D0},	//#TVAR_ash_pGAS[12]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[13]
	{0x0F12, 0x016A},	//#TVAR_ash_pGAS[14]
	{0x0F12, 0x0126},	//#TVAR_ash_pGAS[15]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[16]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[17]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[18]
	{0x0F12, 0x009B},	//#TVAR_ash_pGAS[19]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[20]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[21]
	{0x0F12, 0x00EF},	//#TVAR_ash_pGAS[22]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[23]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[24]
	{0x0F12, 0x019C},	//#TVAR_ash_pGAS[25]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[26]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[27]
	{0x0F12, 0x00E2},	//#TVAR_ash_pGAS[28]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[29]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[30]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[31]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[32]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[33]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[34]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[35]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[36]
	{0x0F12, 0x0133},	//#TVAR_ash_pGAS[37]
	{0x0F12, 0x016E},	//#TVAR_ash_pGAS[38]
	{0x0F12, 0x0140},	//#TVAR_ash_pGAS[39]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[40]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[41]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[42]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[43]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[44]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[45]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[46]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[47]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[48]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[49]
	{0x0F12, 0x010D},	//#TVAR_ash_pGAS[50]
	{0x0F12, 0x0157},	//#TVAR_ash_pGAS[51]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[52]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[53]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[54]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[55]
	{0x0F12, 0x002F},	//#TVAR_ash_pGAS[56]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[57]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[58]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[59]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[60]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[61]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[62]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[63]
	{0x0F12, 0x0142},	//#TVAR_ash_pGAS[64]
	{0x0F12, 0x011A},	//#TVAR_ash_pGAS[65]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[66]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[67]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[68]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[69]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[70]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[71]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[72]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[73]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[74]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[75]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[76]
	{0x0F12, 0x013B},	//#TVAR_ash_pGAS[77]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[78]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[79]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[80]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[81]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[82]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[83]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[84]
	{0x0F12, 0x0014},	//#TVAR_ash_pGAS[85]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[86]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[87]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[88]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[89]
	{0x0F12, 0x013F},	//#TVAR_ash_pGAS[90]
	{0x0F12, 0x012C},	//#TVAR_ash_pGAS[91]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[92]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[93]
	{0x0F12, 0x0067},	//#TVAR_ash_pGAS[94]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[95]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[96]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[97]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[98]
	{0x0F12, 0x004B},	//#TVAR_ash_pGAS[99]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[100]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[101]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[102]
	{0x0F12, 0x0150},	//#TVAR_ash_pGAS[103]
	{0x0F12, 0x0149},	//#TVAR_ash_pGAS[104]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[105]
	{0x0F12, 0x00C5},	//#TVAR_ash_pGAS[106]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[107]
	{0x0F12, 0x0061},	//#TVAR_ash_pGAS[108]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[109]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[110]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[111]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[112]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[113]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[114]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[115]
	{0x0F12, 0x016D},	//#TVAR_ash_pGAS[116]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[117]
	{0x0F12, 0x013A},	//#TVAR_ash_pGAS[118]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[119]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[120]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[121]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[122]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[123]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[124]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[125]
	{0x0F12, 0x00D8},	//#TVAR_ash_pGAS[126]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[127]
	{0x0F12, 0x0154},	//#TVAR_ash_pGAS[128]
	{0x0F12, 0x0186},	//#TVAR_ash_pGAS[129]
	{0x0F12, 0x018F},	//#TVAR_ash_pGAS[130]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[131]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[132]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[133]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[134]
	{0x0F12, 0x00B4},	//#TVAR_ash_pGAS[135]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[136]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[137]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[138]
	{0x0F12, 0x0112},	//#TVAR_ash_pGAS[139]
	{0x0F12, 0x014F},	//#TVAR_ash_pGAS[140]
	{0x0F12, 0x0185},	//#TVAR_ash_pGAS[141]
	{0x0F12, 0x01A1},	//#TVAR_ash_pGAS[142]
	{0x0F12, 0x0168},	//#TVAR_ash_pGAS[143]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[144]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[145]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[146]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[147]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[148]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[149]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[150]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[151]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[152]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[153]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[154]
	{0x0F12, 0x0163},	//#TVAR_ash_pGAS[155]
	{0x0F12, 0x0131},	//#TVAR_ash_pGAS[156]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[157]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[158]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[159]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[160]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[161]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[162]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[163]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[164]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[165]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[166]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[167]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[168]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[169]
	{0x0F12, 0x00D2},	//#TVAR_ash_pGAS[170]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[171]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[172]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[173]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[174]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[175]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[176]
	{0x0F12, 0x0059},	//#TVAR_ash_pGAS[177]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[178]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[179]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[180]
	{0x0F12, 0x010C},	//#TVAR_ash_pGAS[181]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[182]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[183]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[184]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[185]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[186]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[187]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[188]
	{0x0F12, 0x0025},	//#TVAR_ash_pGAS[189]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[190]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[191]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[192]
	{0x0F12, 0x00C2},	//#TVAR_ash_pGAS[193]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[194]
	{0x0F12, 0x00DD},	//#TVAR_ash_pGAS[195]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[196]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[197]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[198]
	{0x0F12, 0x001E},	//#TVAR_ash_pGAS[199]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[200]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[201]
	{0x0F12, 0x0010},	//#TVAR_ash_pGAS[202]
	{0x0F12, 0x0026},	//#TVAR_ash_pGAS[203]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[204]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[205]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[206]
	{0x0F12, 0x00EC},	//#TVAR_ash_pGAS[207]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[208]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[209]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[210]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[211]
	{0x0F12, 0x0017},	//#TVAR_ash_pGAS[212]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[213]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[214]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[215]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[216]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[217]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[218]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[219]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[220]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[221]
	{0x0F12, 0x009A},	//#TVAR_ash_pGAS[222]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[223]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[224]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[225]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[226]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[227]
	{0x0F12, 0x000F},	//#TVAR_ash_pGAS[228]
	{0x0F12, 0x0027},	//#TVAR_ash_pGAS[229]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[230]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[231]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[232]
	{0x0F12, 0x00F1},	//#TVAR_ash_pGAS[233]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[234]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[235]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[236]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[237]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[238]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[239]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[240]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[241]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[242]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[243]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[244]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[245]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[246]
	{0x0F12, 0x00FA},	//#TVAR_ash_pGAS[247]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[248]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[249]
	{0x0F12, 0x0064},	//#TVAR_ash_pGAS[250]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[251]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[252]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[253]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[254]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[255]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[256]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[257]
	{0x0F12, 0x00DF},	//#TVAR_ash_pGAS[258]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[259]
	{0x0F12, 0x0110},	//#TVAR_ash_pGAS[260]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[261]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[262]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[263]
	{0x0F12, 0x006E},	//#TVAR_ash_pGAS[264]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[265]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[266]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[267]
	{0x0F12, 0x0083},	//#TVAR_ash_pGAS[268]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[269]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[270]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[271]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[272]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[273]
	{0x0F12, 0x0104},	//#TVAR_ash_pGAS[274]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[275]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[276]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[277]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[278]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[279]
	{0x0F12, 0x0098},	//#TVAR_ash_pGAS[280]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[281]
	{0x0F12, 0x00D1},	//#TVAR_ash_pGAS[282]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[283]
	{0x0F12, 0x0127},	//#TVAR_ash_pGAS[284]
	{0x0F12, 0x0146},	//#TVAR_ash_pGAS[285]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[286]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[287]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[288]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[289]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[290]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[291]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[292]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[293]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[294]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[295]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[296]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[297]
	{0x0F12, 0x0180},	//#TVAR_ash_pGAS[298]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[299]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[300]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[301]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[302]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[303]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[304]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[305]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[306]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[307]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[308]
	{0x0F12, 0x00EE},	//#TVAR_ash_pGAS[309]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[310]
	{0x0F12, 0x014C},	//#TVAR_ash_pGAS[311]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[312]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[313]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[314]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[315]
	{0x0F12, 0x0057},	//#TVAR_ash_pGAS[316]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[317]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[318]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[319]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[320]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[321]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[322]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[323]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[324]
	{0x0F12, 0x00F6},	//#TVAR_ash_pGAS[325]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[326]
	{0x0F12, 0x0081},	//#TVAR_ash_pGAS[327]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[328]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[329]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[330]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[331]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[332]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[333]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[334]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[335]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[336]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[337]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[338]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[339]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[340]
	{0x0F12, 0x0041},	//#TVAR_ash_pGAS[341]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[342]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[343]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[344]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[345]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[346]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[347]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[348]
	{0x0F12, 0x00BA},	//#TVAR_ash_pGAS[349]
	{0x0F12, 0x00F7},	//#TVAR_ash_pGAS[350]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[351]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[352]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[353]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[354]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[355]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[356]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[357]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[358]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[359]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[360]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[361]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[362]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[363]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[364]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[365]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[366]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[367]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[368]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[369]
	{0x0F12, 0x0002},	//#TVAR_ash_pGAS[370]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[371]
	{0x0F12, 0x0023},	//#TVAR_ash_pGAS[372]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[373]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[374]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[375]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[376]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[377]
	{0x0F12, 0x00B0},	//#TVAR_ash_pGAS[378]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[379]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[380]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[381]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[382]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[383]
	{0x0F12, 0x001C},	//#TVAR_ash_pGAS[384]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[385]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[386]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[387]
	{0x0F12, 0x00B7},	//#TVAR_ash_pGAS[388]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[389]
	{0x0F12, 0x0106},	//#TVAR_ash_pGAS[390]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[391]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[392]
	{0x0F12, 0x006C},	//#TVAR_ash_pGAS[393]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[394]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[395]
	{0x0F12, 0x0033},	//#TVAR_ash_pGAS[396]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[397]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[398]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[399]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[400]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[401]
	{0x0F12, 0x0101},	//#TVAR_ash_pGAS[402]
	{0x0F12, 0x0120},	//#TVAR_ash_pGAS[403]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[404]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[405]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[406]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[407]
	{0x0F12, 0x0062},	//#TVAR_ash_pGAS[408]
	{0x0F12, 0x005C},	//#TVAR_ash_pGAS[409]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[410]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[411]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[412]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[413]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[414]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[415]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[416]
	{0x0F12, 0x0115},	//#TVAR_ash_pGAS[417]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[418]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[419]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[420]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[421]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[422]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[423]
	{0x0F12, 0x00A4},	//#TVAR_ash_pGAS[424]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[425]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[426]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[427]
	{0x0F12, 0x0130},	//#TVAR_ash_pGAS[428]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[429]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[430]
	{0x0F12, 0x00C6},	//#TVAR_ash_pGAS[431]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[432]
	{0x0F12, 0x0090},	//#TVAR_ash_pGAS[433]
	{0x0F12, 0x0085},	//#TVAR_ash_pGAS[434]
	{0x0F12, 0x0082},	//#TVAR_ash_pGAS[435]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[436]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[437]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[438]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[439]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[440]
	{0x0F12, 0x0135},	//#TVAR_ash_pGAS[441]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[442]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[443]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[444]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[445]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[446]
	{0x0F12, 0x005D},	//#TVAR_ash_pGAS[447]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[448]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[449]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[450]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[451]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[452]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[453]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[454]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[455]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[456]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[457]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[458]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[459]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[460]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[461]
	{0x0F12, 0x003D},	//#TVAR_ash_pGAS[462]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[463]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[464]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[465]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[466]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[467]
	{0x0F12, 0x00B5},	//#TVAR_ash_pGAS[468]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[469]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[470]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[471]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[472]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[473]
	{0x0F12, 0x001A},	//#TVAR_ash_pGAS[474]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[475]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[476]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[477]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[478]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[479]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[480]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[481]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[482]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[483]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[484]
	{0x0F12, 0x0018},	//#TVAR_ash_pGAS[485]
	{0x0F12, 0x000A},	//#TVAR_ash_pGAS[486]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[487]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[488]
	{0x0F12, 0x001F},	//#TVAR_ash_pGAS[489]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[490]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[491]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[492]
	{0x0F12, 0x00B6},	//#TVAR_ash_pGAS[493]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[494]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[495]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[496]
	{0x0F12, 0x0028},	//#TVAR_ash_pGAS[497]
	{0x0F12, 0x0012},	//#TVAR_ash_pGAS[498]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[499]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[500]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[501]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[502]
	{0x0F12, 0x0030},	//#TVAR_ash_pGAS[503]
	{0x0F12, 0x0050},	//#TVAR_ash_pGAS[504]
	{0x0F12, 0x007B},	//#TVAR_ash_pGAS[505]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[506]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[507]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[508]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[509]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[510]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[511]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[512]
	{0x0F12, 0x0001},	//#TVAR_ash_pGAS[513]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[514]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[515]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[516]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[517]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[518]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[519]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[520]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[521]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[522]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[523]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[524]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[525]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[526]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[527]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[528]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[529]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[530]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[531]
	{0x0F12, 0x00AE},	//#TVAR_ash_pGAS[532]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[533]
	{0x0F12, 0x0097},	//#TVAR_ash_pGAS[534]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[535]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[536]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[537]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[538]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[539]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[540]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[541]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[542]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[543]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[544]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[545]
	{0x0F12, 0x00D3},	//#TVAR_ash_pGAS[546]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[547]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[548]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[549]
	{0x0F12, 0x0058},	//#TVAR_ash_pGAS[550]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[551]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[552]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[553]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[554]
	{0x0F12, 0x006D},	//#TVAR_ash_pGAS[555]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[556]
	{0x0F12, 0x00AC},	//#TVAR_ash_pGAS[557]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[558]
	{0x0F12, 0x00F5},	//#TVAR_ash_pGAS[559]
	{0x0F12, 0x00D4},	//#TVAR_ash_pGAS[560]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[561]
	{0x0F12, 0x008E},	//#TVAR_ash_pGAS[562]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[563]
	{0x0F12, 0x0070},	//#TVAR_ash_pGAS[564]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[565]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[566]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[567]
	{0x0F12, 0x0093},	//#TVAR_ash_pGAS[568]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[569]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[570]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[571]



	//=================================================================================================
	//	Set Gamma LUT
	//=================================================================================================
	//							//New	//Hong	//ORG
	{0x002A, 0x04CC},
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[0][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[0][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[0][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[0][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[0][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[0][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[0][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[0][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[0][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[0][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[0][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[0][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[0][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[0][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[0][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[0][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[1][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[1][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[1][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[1][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[1][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[1][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[1][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[1][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[1][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[1][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[1][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[1][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[1][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[1][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[1][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[1][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[2][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[2][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[2][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[2][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[2][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[2][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[2][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[2][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[2][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[2][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[2][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[2][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[2][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[2][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[2][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[2][15]



	//=================================================================================================
	//	Set AWB
	//=================================================================================================
	{0x002A, 0x0DA8},
	{0x0F12, 0x0020},	// #awbb_LowBr0_NBzone	// 2008.04.24. eliminate to the purple color at low light.
	{0x002A, 0x0E8C},
	{0x0F12, 0x0000},	// #awbb_LowBr0_PatchNumZone
	{0x002A, 0x0D6C},
	{0x0F12, 0x0040},	// #awbb_YMedMoveToYAv

	// Indoor Gray Zone
	{0x002A, 0x0B9C},
	{0x0F12, 0x0339},	//0392	// #awbb_IndoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x0393},	//03A4	// #awbb_IndoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x031B},	//036F	// #awbb_IndoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x039C},	//03AE	// #awbb_IndoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x030A},	//034E	// #awbb_IndoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0398},	//03A2	// #awbb_IndoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x02F3},	//0331	// #awbb_IndoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0392},	//0392	// #awbb_IndoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x02E0},	//0315	// #awbb_IndoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0388},	//0381	// #awbb_IndoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x02CB},	//02F8	// #awbb_IndoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0374},	//036C	// #awbb_IndoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x02BB},	//02E1	// #awbb_IndoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x034B},	//0358	// #awbb_IndoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x02AA},	//02C5	// #awbb_IndoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x0336},	//033B	// #awbb_IndoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x029C},	//02B1	// #awbb_IndoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0321},	//0323	// #awbb_IndoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x028B},	//029C	// #awbb_IndoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x030B},	//030F	// #awbb_IndoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x027B},	//028B	// #awbb_IndoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02FD},	//02FA	// #awbb_IndoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x0268},	//027C	// #awbb_IndoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x02EA},	//02E2	// #awbb_IndoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x0259},	//0268	// #awbb_IndoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x02D8},	//02D1	// #awbb_IndoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x024C},	//0259	// #awbb_IndoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x02CB},	//02BF	// #awbb_IndoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x023D},	//024C	// #awbb_IndoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x02BD},	//02B1	// #awbb_IndoorGrZones_m_BGrid_14__m_right
	{0x0F12, 0x0231},	//023D	// #awbb_IndoorGrZones_m_BGrid_15__m_left
	{0x0F12, 0x02B1},	//02A2	// #awbb_IndoorGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0222},	//0231	// #awbb_IndoorGrZones_m_BGrid_16__m_left
	{0x0F12, 0x02A8},	//0299	// #awbb_IndoorGrZones_m_BGrid_16__m_right
	{0x0F12, 0x0215},	//0222	// #awbb_IndoorGrZones_m_BGrid_17__m_left
	{0x0F12, 0x029B},	//0290	// #awbb_IndoorGrZones_m_BGrid_17__m_right
	{0x0F12, 0x0208},	//0215	// #awbb_IndoorGrZones_m_BGrid_18__m_left
	{0x0F12, 0x0290},	//0288	// #awbb_IndoorGrZones_m_BGrid_18__m_right
	{0x0F12, 0x01F7},	//0208	// #awbb_IndoorGrZones_m_BGrid_19__m_left
	{0x0F12, 0x0287},	//0281	// #awbb_IndoorGrZones_m_BGrid_19__m_right
	{0x0F12, 0x01E6},	//01F7	// #awbb_IndoorGrZones_m_BGrid_20__m_left
	{0x0F12, 0x027F},	//0279	// #awbb_IndoorGrZones_m_BGrid_20__m_right
	{0x0F12, 0x01D9},	//01E6	// #awbb_IndoorGrZones_m_BGrid_21__m_left
	{0x0F12, 0x0279},	//026E	// #awbb_IndoorGrZones_m_BGrid_21__m_right
	{0x0F12, 0x01CC},	//01D9	// #awbb_IndoorGrZones_m_BGrid_22__m_left
	{0x0F12, 0x0272},	//0268	// #awbb_IndoorGrZones_m_BGrid_22__m_right
	{0x0F12, 0x01C1},	//01CC	// #awbb_IndoorGrZones_m_BGrid_23__m_left
	{0x0F12, 0x026C},	//0263	// #awbb_IndoorGrZones_m_BGrid_23__m_right
	{0x0F12, 0x01B3},	//01C1	// #awbb_IndoorGrZones_m_BGrid_24__m_left
	{0x0F12, 0x0265},	//0260	// #awbb_IndoorGrZones_m_BGrid_24__m_right
	{0x0F12, 0x01AA},	//01B3	// #awbb_IndoorGrZones_m_BGrid_25__m_left
	{0x0F12, 0x025C},	//0259	// #awbb_IndoorGrZones_m_BGrid_25__m_right
	{0x0F12, 0x019F},	//01AA	// #awbb_IndoorGrZones_m_BGrid_26__m_left
	{0x0F12, 0x0250},	//0253	// #awbb_IndoorGrZones_m_BGrid_26__m_right
	{0x0F12, 0x019B},	//019F	// #awbb_IndoorGrZones_m_BGrid_27__m_left
	{0x0F12, 0x0247},	//0248	// #awbb_IndoorGrZones_m_BGrid_27__m_right
	{0x0F12, 0x0197},	//019B	// #awbb_IndoorGrZones_m_BGrid_28__m_left
	{0x0F12, 0x0239},	//023F	// #awbb_IndoorGrZones_m_BGrid_28__m_right
	{0x0F12, 0x019B},	//0197	// #awbb_IndoorGrZones_m_BGrid_29__m_left
	{0x0F12, 0x021D},	//0239	// #awbb_IndoorGrZones_m_BGrid_29__m_right
	{0x0F12, 0x019D},	//019B	// #awbb_IndoorGrZones_m_BGrid_30__m_left
	{0x0F12, 0x0202},	//021D	// #awbb_IndoorGrZones_m_BGrid_30__m_right
	{0x0F12, 0x01AC},	//019D	// #awbb_IndoorGrZones_m_BGrid_31__m_left
	{0x0F12, 0x01D3},	//020A	// #awbb_IndoorGrZones_m_BGrid_31__m_right
	{0x0F12, 0x0000},	//01AC	// #awbb_IndoorGrZones_m_BGrid_32__m_left
	{0x0F12, 0x0000},	//01D3	// #awbb_IndoorGrZones_m_BGrid_32__m_right
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_left
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_right

	{0x002A, 0x0C3C},
	{0x0F12, 0x0004},	// #awbb_IndoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0138},	// #awbb_IndoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x0F12, 0x0000},

	// Outdoor Gray Zone
	{0x0F12, 0x0255},	//#awbb_OutdoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x0236},	//#awbb_OutdoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x0290},	//#awbb_OutdoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x0227},	//#awbb_OutdoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0297},	//#awbb_OutdoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x0210},	//#awbb_OutdoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0293},	//#awbb_OutdoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0200},	//#awbb_OutdoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x028B},	//#awbb_OutdoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x01F1},	//#awbb_OutdoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0286},	//#awbb_OutdoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x0282},	//#awbb_OutdoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x01E0},	//#awbb_OutdoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x027B},	//#awbb_OutdoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x01DE},	//#awbb_OutdoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0274},	//#awbb_OutdoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01DF},	//#awbb_OutdoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x0263},	//#awbb_OutdoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01F6},	//#awbb_OutdoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x0253},	//#awbb_OutdoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x020B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x023B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_right

	{0x002A, 0x0CB8},
	{0x0F12, 0x0004},	// #awbb_OutdoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x000C},	//#awbb_OutdoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0217},	//#awbb_OutdoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high
	{0x0F12, 0x0000},

	// Low Brightness Gray Zone
	{0x0F12, 0x0324},	//031E  // #awbb_LowBrGrZones_m_BGrid_0__m_left
	{0x0F12, 0x03A1},	//037E  // #awbb_LowBrGrZones_m_BGrid_0__m_right
	{0x0F12, 0x02F2},	//02CF  // #awbb_LowBrGrZones_m_BGrid_1__m_left
	{0x0F12, 0x03BA},	//0389  // #awbb_LowBrGrZones_m_BGrid_1__m_right
	{0x0F12, 0x02C4},	//02A6  // #awbb_LowBrGrZones_m_BGrid_2__m_left
	{0x0F12, 0x03B6},	//0386  // #awbb_LowBrGrZones_m_BGrid_2__m_right
	{0x0F12, 0x029E},	//0282  // #awbb_LowBrGrZones_m_BGrid_3__m_left
	{0x0F12, 0x03A6},	//0374  // #awbb_LowBrGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0283},	//0259  // #awbb_LowBrGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0380},	//0352  // #awbb_LowBrGrZones_m_BGrid_4__m_right
	{0x0F12, 0x025F},	//0239  // #awbb_LowBrGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0344},	//0330  // #awbb_LowBrGrZones_m_BGrid_5__m_right
	{0x0F12, 0x0241},	//021E  // #awbb_LowBrGrZones_m_BGrid_6__m_left
	{0x0F12, 0x031F},	//031B  // #awbb_LowBrGrZones_m_BGrid_6__m_right
	{0x0F12, 0x0227},	//020B  // #awbb_LowBrGrZones_m_BGrid_7__m_left
	{0x0F12, 0x02FC},	//0300  // #awbb_LowBrGrZones_m_BGrid_7__m_right
	{0x0F12, 0x020A},	//01F1  // #awbb_LowBrGrZones_m_BGrid_8__m_left
	{0x0F12, 0x02D9},	//02DD  // #awbb_LowBrGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01F1},	//01E0  // #awbb_LowBrGrZones_m_BGrid_9__m_left
	{0x0F12, 0x02C4},	//02C4  // #awbb_LowBrGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01D0},	//01CD  // #awbb_LowBrGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02B2},	//02AF  // #awbb_LowBrGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01B8},	//019E  // #awbb_LowBrGrZones_m_BGrid_11__m_left
	{0x0F12, 0x029E},	//02A0  // #awbb_LowBrGrZones_m_BGrid_11__m_right
	{0x0F12, 0x019F},	//0187  // #awbb_LowBrGrZones_m_BGrid_12__m_left
	{0x0F12, 0x0290},	//028C  // #awbb_LowBrGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0189},	//017D  // #awbb_LowBrGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0282},	//027E  // #awbb_LowBrGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0182},	//0179  // #awbb_LowBrGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0271},	//0258  // #awbb_LowBrGrZones_m_BGrid_14__m_right
	{0x0F12, 0x017F},	//0186  // #awbb_LowBrGrZones_m_BGrid_15__m_left
	{0x0F12, 0x0254},	//0232  // #awbb_LowBrGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0183},	//01BD  // #awbb_LowBrGrZones_m_BGrid_16__m_left
	{0x0F12, 0x022D},	//01FB  // #awbb_LowBrGrZones_m_BGrid_16__m_right
	{0x0F12, 0x019C},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_left
	{0x0F12, 0x01D3},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_right

	{0x002A, 0x0D34},
	{0x0F12, 0x0005},	// #awbb_LowBrGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0012},	// #awbb_LowBrGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0120},	// #awbb_LowBrGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0002},	// #awbb_LowBrGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D48},
	{0x0F12, 0x00E0},	// #awbb_LowBrGrZones_y_high
	{0x0F12, 0x0000},

	// Lowtemp Circle
	{0x0F12, 0x032F},	// #awbb_CrclLowT_R_c
	{0x0F12, 0x0000},
	{0x0F12, 0x017A},	// #awbb_CrclLowT_B_c
	{0x0F12, 0x0000},
	{0x0F12, 0x7300},	// #awbb_CrclLowT_Rad_c
	{0x0F12, 0x0000},
	{0x0F12, 0x000A},	// #awbb_CrclLowT_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D60},
	{0x0F12, 0x00E0},	// #awbb_CrclLowT_y_high
	{0x0F12, 0x0000},
	{0x002A, 0x0D82},
	{0x0F12, 0x0001},	// #awbb_ByPass_LowTempMode

	// White Locus
	{0x002A, 0x0D66},
	{0x0F12, 0x011F},	// #awbb_IntcR
	{0x0F12, 0x0132},	// #awbb_IntcB
	{0x002A, 0x0D74},
	{0x0F12, 0x052A},	// #awbb_MvEq_RBthresh

	// Gamut Thresholds
	{0x002A, 0x0DAE},
	{0x0F12, 0x0036},	// #awbb_GamutWidthThr2
	{0x0F12, 0x001C},	// #awbb_GamutHeightThr2
	{0x002A, 0x0DAA},
	{0x0F12, 0x071A},	// #awbb_GamutWidthThr1
	{0x0F12, 0x03A4},	// #awbb_GamutHeightThr1

	// SceneDetection Thresholds
	{0x002A, 0x0D92},
	{0x0F12, 0x0CE4},	//0A8C	// #awbb_SunnyBr
	{0x0F12, 0x0096},	// #awbb_Sunny_NBzone
	{0x002A, 0x0E86},
	{0x0F12, 0x0216},	// #awbb_OutdoorWP_r
	{0x0F12, 0x029F},	// #awbb_OutdoorWP_b
	{0x002A, 0x0D96},
	{0x0F12, 0x0580},	// #awbb_CloudyBr
	{0x0F12, 0x0096},	// #awbb_Cloudy_NBzone
	{0x002A, 0x0DB2},
	{0x0F12, 0x00DA},	// #awbb_CloudyRB
	{0x002A, 0x0D9A},
	{0x0F12, 0x000A},	// #awbb_Cloudy_BdivRzone
	{0x002A, 0x0DB4},
	{0x0F12, 0x0459},	// #awbb_LowTempRB
	{0x002A, 0x0DA4},
	{0x0F12, 0x000E},	// #awbb_LowTemp_RBzone
	{0x002A, 0x0D64},
	{0x0F12, 0x0040},	// #awbb_DarkBr
	{0x002A, 0x0DA6},
	{0x0F12, 0x001E},	// #awbb_LowBr_NBzone
	{0x002A, 0x0D9C},
	{0x0F12, 0x001B},	// #awbb_MacbethGamut_WidthZone
	{0x0F12, 0x000E},	// #awbb_MacbethGamut_HeightZone
	{0x0F12, 0x0008},	// #awbb_MacbethGamut_WidthZone2
	{0x0F12, 0x0004},	// #awbb_MacbethGamut_HeightZone2

	// AWB Debug.(Outdoor Pink)
	{0x002A, 0x0E30},
	{0x0F12, 0x0000},	//	#awbb_OutdoorFltrSz (outdoor WB를 moving average filtering)

	// UseInvalidOutdoor option
	{0x002A, 0x0D88},
	{0x0F12, 0x0001},	// #awbb_Use_InvalidOutDoor

	// AWB input Y-Filter setting
	{0x002A, 0x0C48},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x002A, 0x0CC4},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high

	{0x002A, 0x0DC2},
	{0x0F12, 0x0030},	// #awbb_GnCurPntImmunity
	{0x0F12, 0x00C8},	// #awbb_GnFarPntImmunity
	{0x0F12, 0x012C},	// #awbb_GnCurPntLongJump
	{0x0F12, 0x0258},	// #awbb_GainsMaxMove
	{0x0F12, 0x0003},	// #awbb_GnMinMatchToJump

	// RB Gain Offset
	//WRITE 70000E32	FFE0	//#awbb_RGainOff
	//WRITE 70000E34	FFe0	//#awbb_BGainOff


	//=================================================================================================
	//	Set CCM
	//=================================================================================================
	// CCM Start Address
	{0x002A, 0x06D0},
	{0x0F12, 0x2800},   //#TVAR_wbt_pBaseCcmsAddr[0]
	{0x0F12, 0x7000},
	{0x0F12, 0x2824},   //#TVAR_wbt_pBaseCcmsAddr[1]
	{0x0F12, 0x7000},
	{0x0F12, 0x2848},   //#TVAR_wbt_pBaseCcmsAddr[2]
	{0x0F12, 0x7000},
	{0x0F12, 0x286C},   //#TVAR_wbt_pBaseCcmsAddr[3]
	{0x0F12, 0x7000},
	{0x0F12, 0x2890},   //#TVAR_wbt_pBaseCcmsAddr[4]
	{0x0F12, 0x7000},
	{0x0F12, 0x28B4},   //#TVAR_wbt_pBaseCcmsAddr[5]
	{0x0F12, 0x7000},
	{0x002A, 0x06EC},
	{0x0F12, 0x28D8},   //#TVAR_wbt_pOutdoorCcm
	{0x0F12, 0x7000},

	{0x002A, 0x2800},
	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	// Outdoor CCM
	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEA4},
	{0x0F12, 0x020F},
	{0x0F12, 0xFF8F},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},



	//=================================================================================================
	//	Set NB
	//=================================================================================================
	{0x002A, 0x07EA},
	{0x0F12, 0x0000},	//afit_bUseNoiseInd 0 : NB 1: Noise Index

	// param_start	SARR_uNormBrInDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrInDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrInDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrInDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrInDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrInDoor[4]

	// param_start	SARR_uNormBrOutDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrOutDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrOutDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrOutDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrOutDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrOutDoor[4]



	//=================================================================================================
	//	Set AFIT
	//=================================================================================================
	// AFIT Start Address
	{0x002A, 0x0814},
	{0x0F12, 0x082C},	//#TVAR_afit_pBaseVals
	{0x0F12, 0x7000},	//#TVAR_afit_pBaseVals

	// AFIT Table	//0223
	{0x002A, 0x082C},
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0000},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0028},	//SHARP_BLUR
	{0x0F12, 0xFFE2},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0028},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                   bin: desparity low
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"                bin: desparity high
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x06FF},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x030A},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"               ]negati fine
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"                  ]low fine
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"                ]high fine
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"                   ]high low thres
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0005},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01AA},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0000},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x001E},	//SHARP_BLUR
	{0x0F12, 0xFFE2},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x000E},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                  bin: desparity low
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x08B7},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0308},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01A0},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x000E},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0089},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0202},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0202},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0206},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x803C},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0006},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0205},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0000},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8050},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0006},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0000},	//UVDenoise_iYLowThresh
	{0x0F12, 0x0000},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0105},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0200},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x805A},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[0]
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[1]
	{0x0F12, 0x0800},	// # afit_pConstBaseVals[2]
	{0x0F12, 0x0600},	// # afit_pConstBaseVals[3]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[4]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[5]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[6]
	{0x0F12, 0x0300},	// # afit_pConstBaseVals[7]
	{0x0F12, 0x0002},	// # afit_pConstBaseVals[8]
	{0x0F12, 0x0400},	// # afit_pConstBaseVals[9]
	{0x0F12, 0x0106},	// # afit_pConstBaseVals[10]
	{0x0F12, 0x0005},	// # afit_pConstBaseVals[11]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[12]
	{0x0F12, 0x0003},	// # afit_pConstBaseVals[13]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[14]
	{0x0F12, 0xFFD6},	// # afit_pConstBaseVals[15]
	{0x0F12, 0x53C1},	// # afit_pConstBaseVals[16]
	{0x0F12, 0xE1FE},	// # afit_pConstBaseVals[17]
	{0x0F12, 0x0001},	// # afit_pConstBaseVals[18]

	{0x002A, 0x01EA},
	{0x0F12, 0x0000},	//#REG_TC_UserSharpBlur
	{0x002A, 0x0E32},
	{0x0F12, 0xFFF0},	//#awbb_RGainOff
	{0x0F12, 0x0000},	//#awbb_BGainOff

	// For fast AE
	{0x002A, 0x0FAC},
	{0x0F12, 0x0001},	// AE Gain output [0]    default : 0x10
	{0x0F12, 0x0008},	// AE Gain output [1]    default : 0x20
	{0x0F12, 0x0015},	// AE Gain output [2]    default : 0x40

	// For fast AWB
	{0x002A, 0x0DCA},
	{0x0F12, 0x0001},	// awbb_GnMinMatchToJump = 0x02 (default:0x03)
	{0x0028, 0x7000},
	{0x002A, 0x0E98},
	{0x0F12, 0x0300},	//bp_uMaxBrightnessFactor 0180 -> 0300
	{0x002A, 0x0E9E},
	{0x0F12, 0x0300},	//bp_uMinBrightnessFactor 0180 -> 0300

	// Update Changed Registers
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},

	{0x0028, 0xD000},
	{0x002A, 0x1088},	// PCLK_CD
	{0x0F12, 0x05D5},

	{0x0028, 0xD000},
	{0x002A, 0x1094},	//PCLK_Delay
	{0x0F12, 0x0030},

	{0x0028, 0xD000},
	{0x002A, 0x1082},	// Data CD
	{0x0F12, 0x03FF},

	{0x0028, 0xD000},
	{0x002A, 0x1084},	// Data CD
	{0x0F12, 0x03FF},


    {SEQUENCE_WAIT_MS, 0x0064},
    {SEQUENCE_END, 0x0000}
};



const i2c_cam_reg16 S5k5bafx_init_640x480_30fps[] =
{
	//=================================================================================================
	//	ARM Go
	//=================================================================================================
	{0x0028, 0xD000},
	{0x002A, 0x1030},
	{0x0F12, 0x0000},
	{0x002A, 0x0014},
	{0x0F12, 0x0001},

	{0xFFFE, 0x0014},// Delay 100 -> 20

	{0x0028, 0x7000},
	{0x002A, 0x2080},
	{0x0F12, 0xB510},	// 70002080
	{0x0F12, 0xF000},	// 70002082
	{0x0F12, 0xF8FC},	// 70002084
	{0x0F12, 0xBC10},	// 70002086
	{0x0F12, 0xBC08},	// 70002088
	{0x0F12, 0x4718},	// 7000208A
	{0x0F12, 0xB5FE},	// 7000208C
	{0x0F12, 0x0007},	// 7000208E
	{0x0F12, 0x683C},	// 70002090
	{0x0F12, 0x687E},	// 70002092
	{0x0F12, 0x1DA5},	// 70002094
	{0x0F12, 0x88A0},	// 70002096
	{0x0F12, 0x2800},	// 70002098
	{0x0F12, 0xD00B},	// 7000209A
	{0x0F12, 0x88A8},	// 7000209C
	{0x0F12, 0x2800},	// 7000209E
	{0x0F12, 0xD008},	// 700020A0
	{0x0F12, 0x8820},	// 700020A2
	{0x0F12, 0x8829},	// 700020A4
	{0x0F12, 0x4288},	// 700020A6
	{0x0F12, 0xD301},	// 700020A8
	{0x0F12, 0x1A40},	// 700020AA
	{0x0F12, 0xE000},	// 700020AC
	{0x0F12, 0x1A08},	// 700020AE
	{0x0F12, 0x9001},	// 700020B0
	{0x0F12, 0xE001},	// 700020B2
	{0x0F12, 0x2019},	// 700020B4
	{0x0F12, 0x9001},	// 700020B6
	{0x0F12, 0x497B},	// 700020B8
	{0x0F12, 0x466B},	// 700020BA
	{0x0F12, 0x8A48},	// 700020BC
	{0x0F12, 0x8118},	// 700020BE
	{0x0F12, 0x8A88},	// 700020C0
	{0x0F12, 0x8158},	// 700020C2
	{0x0F12, 0x4879},	// 700020C4
	{0x0F12, 0x8940},	// 700020C6
	{0x0F12, 0x0040},	// 700020C8
	{0x0F12, 0x2103},	// 700020CA
	{0x0F12, 0xF000},	// 700020CC
	{0x0F12, 0xF902},	// 700020CE
	{0x0F12, 0x88A1},	// 700020D0
	{0x0F12, 0x4288},	// 700020D2
	{0x0F12, 0xD908},	// 700020D4
	{0x0F12, 0x8828},	// 700020D6
	{0x0F12, 0x8030},	// 700020D8
	{0x0F12, 0x8868},	// 700020DA
	{0x0F12, 0x8070},	// 700020DC
	{0x0F12, 0x88A8},	// 700020DE
	{0x0F12, 0x6038},	// 700020E0
	{0x0F12, 0xBCFE},	// 700020E2
	{0x0F12, 0xBC08},	// 700020E4
	{0x0F12, 0x4718},	// 700020E6
	{0x0F12, 0x88A9},	// 700020E8
	{0x0F12, 0x4288},	// 700020EA
	{0x0F12, 0xD906},	// 700020EC
	{0x0F12, 0x8820},	// 700020EE
	{0x0F12, 0x8030},	// 700020F0
	{0x0F12, 0x8860},	// 700020F2
	{0x0F12, 0x8070},	// 700020F4
	{0x0F12, 0x88A0},	// 700020F6
	{0x0F12, 0x6038},	// 700020F8
	{0x0F12, 0xE7F2},	// 700020FA
	{0x0F12, 0x9801},	// 700020FC
	{0x0F12, 0xA902},	// 700020FE
	{0x0F12, 0xF000},	// 70002100
	{0x0F12, 0xF8EE},	// 70002102
	{0x0F12, 0x0033},	// 70002104
	{0x0F12, 0x0029},	// 70002106
	{0x0F12, 0x9A02},	// 70002108
	{0x0F12, 0x0020},	// 7000210A
	{0x0F12, 0xF000},	// 7000210C
	{0x0F12, 0xF8F0},	// 7000210E
	{0x0F12, 0x6038},	// 70002110
	{0x0F12, 0xE7E6},	// 70002112
	{0x0F12, 0xB5F0},	// 70002114
	{0x0F12, 0xB08B},	// 70002116
	{0x0F12, 0x0006},	// 70002118
	{0x0F12, 0x2000},	// 7000211A
	{0x0F12, 0x9004},	// 7000211C
	{0x0F12, 0x6835},	// 7000211E
	{0x0F12, 0x6874},	// 70002120
	{0x0F12, 0x68B0},	// 70002122
	{0x0F12, 0x900A},	// 70002124
	{0x0F12, 0x68F0},	// 70002126
	{0x0F12, 0x9009},	// 70002128
	{0x0F12, 0x4F60},	// 7000212A
	{0x0F12, 0x8979},	// 7000212C
	{0x0F12, 0x084A},	// 7000212E
	{0x0F12, 0x88A8},	// 70002130
	{0x0F12, 0x88A3},	// 70002132
	{0x0F12, 0x4298},	// 70002134
	{0x0F12, 0xD300},	// 70002136
	{0x0F12, 0x0018},	// 70002138
	{0x0F12, 0xF000},	// 7000213A
	{0x0F12, 0xF8E1},	// 7000213C
	{0x0F12, 0x9007},	// 7000213E
	{0x0F12, 0x0021},	// 70002140
	{0x0F12, 0x0028},	// 70002142
	{0x0F12, 0xAA04},	// 70002144
	{0x0F12, 0xF000},	// 70002146
	{0x0F12, 0xF8E3},	// 70002148
	{0x0F12, 0x9006},	// 7000214A
	{0x0F12, 0x88A8},	// 7000214C
	{0x0F12, 0x2800},	// 7000214E
	{0x0F12, 0xD102},	// 70002150
	{0x0F12, 0x27FF},	// 70002152
	{0x0F12, 0x1C7F},	// 70002154
	{0x0F12, 0xE047},	// 70002156
	{0x0F12, 0x88A0},	// 70002158
	{0x0F12, 0x2800},	// 7000215A
	{0x0F12, 0xD101},	// 7000215C
	{0x0F12, 0x2700},	// 7000215E
	{0x0F12, 0xE042},	// 70002160
	{0x0F12, 0x8820},	// 70002162
	{0x0F12, 0x466B},	// 70002164
	{0x0F12, 0x8198},	// 70002166
	{0x0F12, 0x8860},	// 70002168
	{0x0F12, 0x81D8},	// 7000216A
	{0x0F12, 0x8828},	// 7000216C
	{0x0F12, 0x8118},	// 7000216E
	{0x0F12, 0x8868},	// 70002170
	{0x0F12, 0x8158},	// 70002172
	{0x0F12, 0xA802},	// 70002174
	{0x0F12, 0xC803},	// 70002176
	{0x0F12, 0xF000},	// 70002178
	{0x0F12, 0xF8D2},	// 7000217A
	{0x0F12, 0x9008},	// 7000217C
	{0x0F12, 0x8ABA},	// 7000217E
	{0x0F12, 0x9808},	// 70002180
	{0x0F12, 0x466B},	// 70002182
	{0x0F12, 0x4342},	// 70002184
	{0x0F12, 0x9202},	// 70002186
	{0x0F12, 0x8820},	// 70002188
	{0x0F12, 0x8198},	// 7000218A
	{0x0F12, 0x8860},	// 7000218C
	{0x0F12, 0x81D8},	// 7000218E
	{0x0F12, 0x980A},	// 70002190
	{0x0F12, 0x9903},	// 70002192
	{0x0F12, 0xF000},	// 70002194
	{0x0F12, 0xF8C4},	// 70002196
	{0x0F12, 0x9A02},	// 70002198
	{0x0F12, 0x17D1},	// 7000219A
	{0x0F12, 0x0E09},	// 7000219C
	{0x0F12, 0x1889},	// 7000219E
	{0x0F12, 0x1209},	// 700021A0
	{0x0F12, 0x4288},	// 700021A2
	{0x0F12, 0xDD1F},	// 700021A4
	{0x0F12, 0x8820},	// 700021A6
	{0x0F12, 0x466B},	// 700021A8
	{0x0F12, 0x8198},	// 700021AA
	{0x0F12, 0x8860},	// 700021AC
	{0x0F12, 0x81D8},	// 700021AE
	{0x0F12, 0x980A},	// 700021B0
	{0x0F12, 0x9903},	// 700021B2
	{0x0F12, 0xF000},	// 700021B4
	{0x0F12, 0xF8B4},	// 700021B6
	{0x0F12, 0x9001},	// 700021B8
	{0x0F12, 0x8828},	// 700021BA
	{0x0F12, 0x466B},	// 700021BC
	{0x0F12, 0x8118},	// 700021BE
	{0x0F12, 0x8868},	// 700021C0
	{0x0F12, 0x8158},	// 700021C2
	{0x0F12, 0x980A},	// 700021C4
	{0x0F12, 0x9902},	// 700021C6
	{0x0F12, 0xF000},	// 700021C8
	{0x0F12, 0xF8AA},	// 700021CA
	{0x0F12, 0x8AB9},	// 700021CC
	{0x0F12, 0x9A08},	// 700021CE
	{0x0F12, 0x4351},	// 700021D0
	{0x0F12, 0x17CA},	// 700021D2
	{0x0F12, 0x0E12},	// 700021D4
	{0x0F12, 0x1851},	// 700021D6
	{0x0F12, 0x120A},	// 700021D8
	{0x0F12, 0x9901},	// 700021DA
	{0x0F12, 0xF000},	// 700021DC
	{0x0F12, 0xF890},	// 700021DE
	{0x0F12, 0x0407},	// 700021E0
	{0x0F12, 0x0C3F},	// 700021E2
	{0x0F12, 0xE000},	// 700021E4
	{0x0F12, 0x2700},	// 700021E6
	{0x0F12, 0x8820},	// 700021E8
	{0x0F12, 0x466B},	// 700021EA
	{0x0F12, 0xAA05},	// 700021EC
	{0x0F12, 0x8198},	// 700021EE
	{0x0F12, 0x8860},	// 700021F0
	{0x0F12, 0x81D8},	// 700021F2
	{0x0F12, 0x8828},	// 700021F4
	{0x0F12, 0x8118},	// 700021F6
	{0x0F12, 0x8868},	// 700021F8
	{0x0F12, 0x8158},	// 700021FA
	{0x0F12, 0xA802},	// 700021FC
	{0x0F12, 0xC803},	// 700021FE
	{0x0F12, 0x003B},	// 70002200
	{0x0F12, 0xF000},	// 70002202
	{0x0F12, 0xF895},	// 70002204
	{0x0F12, 0x88A1},	// 70002206
	{0x0F12, 0x88A8},	// 70002208
	{0x0F12, 0x003A},	// 7000220A
	{0x0F12, 0xF000},	// 7000220C
	{0x0F12, 0xF898},	// 7000220E
	{0x0F12, 0x0004},	// 70002210
	{0x0F12, 0xA804},	// 70002212
	{0x0F12, 0xC803},	// 70002214
	{0x0F12, 0x9A09},	// 70002216
	{0x0F12, 0x9B07},	// 70002218
	{0x0F12, 0xF000},	// 7000221A
	{0x0F12, 0xF889},	// 7000221C
	{0x0F12, 0xA806},	// 7000221E
	{0x0F12, 0xC805},	// 70002220
	{0x0F12, 0x0021},	// 70002222
	{0x0F12, 0xF000},	// 70002224
	{0x0F12, 0xF88C},	// 70002226
	{0x0F12, 0x6030},	// 70002228
	{0x0F12, 0xB00B},	// 7000222A
	{0x0F12, 0xBCF0},	// 7000222C
	{0x0F12, 0xBC08},	// 7000222E
	{0x0F12, 0x4718},	// 70002230
	{0x0F12, 0xB510},	// 70002232
	{0x0F12, 0x6800},	// 70002234
	{0x0F12, 0x2805},	// 70002236
	{0x0F12, 0xD11E},	// 70002238
	{0x0F12, 0x481D},	// 7000223A
	{0x0F12, 0x491D},	// 7000223C
	{0x0F12, 0x8800},	// 7000223E
	{0x0F12, 0x8348},	// 70002240
	{0x0F12, 0x2301},	// 70002242
	{0x0F12, 0x31A0},	// 70002244
	{0x0F12, 0x720B},	// 70002246
	{0x0F12, 0x2000},	// 70002248
	{0x0F12, 0x000A},	// 7000224A
	{0x0F12, 0x3A60},	// 7000224C
	{0x0F12, 0x7150},	// 7000224E
	{0x0F12, 0x71CB},	// 70002250
	{0x0F12, 0x22FF},	// 70002252
	{0x0F12, 0x3980},	// 70002254
	{0x0F12, 0x75CA},	// 70002256
	{0x0F12, 0x764A},	// 70002258
	{0x0F12, 0x000A},	// 7000225A
	{0x0F12, 0x3260},	// 7000225C
	{0x0F12, 0x7210},	// 7000225E
	{0x0F12, 0x7250},	// 70002260
	{0x0F12, 0x2008},	// 70002262
	{0x0F12, 0x7488},	// 70002264
	{0x0F12, 0x74C8},	// 70002266
	{0x0F12, 0x4813},	// 70002268
	{0x0F12, 0x217E},	// 7000226A
	{0x0F12, 0x43C9},	// 7000226C
	{0x0F12, 0x8101},	// 7000226E
	{0x0F12, 0x2306},	// 70002270
	{0x0F12, 0x5EC0},	// 70002272
	{0x0F12, 0xF000},	// 70002274
	{0x0F12, 0xF86C},	// 70002276
	{0x0F12, 0xBC10},	// 70002278
	{0x0F12, 0xBC08},	// 7000227A
	{0x0F12, 0x4718},	// 7000227C
	{0x0F12, 0xB570},	// 7000227E
	{0x0F12, 0x2200},	// 70002280
	{0x0F12, 0x490E},	// 70002282
	{0x0F12, 0x480E},	// 70002284
	{0x0F12, 0x2401},	// 70002286
	{0x0F12, 0xF000},	// 70002288
	{0x0F12, 0xF86A},	// 7000228A
	{0x0F12, 0x0022},	// 7000228C
	{0x0F12, 0x490D},	// 7000228E
	{0x0F12, 0x480D},	// 70002290
	{0x0F12, 0x2502},	// 70002292
	{0x0F12, 0xF000},	// 70002294
	{0x0F12, 0xF864},	// 70002296
	{0x0F12, 0x490C},	// 70002298
	{0x0F12, 0x480D},	// 7000229A
	{0x0F12, 0x002A},	// 7000229C
	{0x0F12, 0xF000},	// 7000229E
	{0x0F12, 0xF85F},	// 700022A0
	{0x0F12, 0xBC70},	// 700022A2
	{0x0F12, 0xBC08},	// 700022A4
	{0x0F12, 0x4718},	// 700022A6
	{0x0F12, 0x1A28},	// 700022A8
	{0x0F12, 0x7000},	// 700022AA
	{0x0F12, 0x0D64},	// 700022AC
	{0x0F12, 0x7000},	// 700022AE
	{0x0F12, 0x2370},	// 700022B0
	{0x0F12, 0x7000},	// 700022B2
	{0x0F12, 0x1C14},	// 700022B4
	{0x0F12, 0x7000},	// 700022B6
	{0x0F12, 0x14FA},	// 700022B8
	{0x0F12, 0x7000},	// 700022BA
	{0x0F12, 0x2115},	// 700022BC
	{0x0F12, 0x7000},	// 700022BE
	{0x0F12, 0x622F},	// 700022C0
	{0x0F12, 0x0000},	// 700022C2
	{0x0F12, 0x208D},	// 700022C4
	{0x0F12, 0x7000},	// 700022C6
	{0x0F12, 0x6445},	// 700022C8
	{0x0F12, 0x0000},	// 700022CA
	{0x0F12, 0x2233},	// 700022CC
	{0x0F12, 0x7000},	// 700022CE
	{0x0F12, 0x3B5B},	// 700022D0
	{0x0F12, 0x0000},	// 700022D2
	{0x0F12, 0x4778},	// 700022D4
	{0x0F12, 0x46C0},	// 700022D6
	{0x0F12, 0xF004},	// 700022D8
	{0x0F12, 0xE51F},	// 700022DA
	{0x0F12, 0xA464},	// 700022DC
	{0x0F12, 0x0000},	// 700022DE
	{0x0F12, 0x4778},	// 700022E0
	{0x0F12, 0x46C0},	// 700022E2
	{0x0F12, 0xC000},	// 700022E4
	{0x0F12, 0xE59F},	// 700022E6
	{0x0F12, 0xFF1C},	// 700022E8
	{0x0F12, 0xE12F},	// 700022EA
	{0x0F12, 0x6009},	// 700022EC
	{0x0F12, 0x0000},	// 700022EE
	{0x0F12, 0x4778},	// 700022F0
	{0x0F12, 0x46C0},	// 700022F2
	{0x0F12, 0xC000},	// 700022F4
	{0x0F12, 0xE59F},	// 700022F6
	{0x0F12, 0xFF1C},	// 700022F8
	{0x0F12, 0xE12F},	// 700022FA
	{0x0F12, 0x622F},	// 700022FC
	{0x0F12, 0x0000},	// 700022FE
	{0x0F12, 0x4778},	// 70002300
	{0x0F12, 0x46C0},	// 70002302
	{0x0F12, 0xC000},	// 70002304
	{0x0F12, 0xE59F},	// 70002306
	{0x0F12, 0xFF1C},	// 70002308
	{0x0F12, 0xE12F},	// 7000230A
	{0x0F12, 0x5F49},	// 7000230C
	{0x0F12, 0x0000},	// 7000230E
	{0x0F12, 0x4778},	// 70002310
	{0x0F12, 0x46C0},	// 70002312
	{0x0F12, 0xC000},	// 70002314
	{0x0F12, 0xE59F},	// 70002316
	{0x0F12, 0xFF1C},	// 70002318
	{0x0F12, 0xE12F},	// 7000231A
	{0x0F12, 0x5FC7},	// 7000231C
	{0x0F12, 0x0000},	// 7000231E
	{0x0F12, 0x4778},	// 70002320
	{0x0F12, 0x46C0},	// 70002322
	{0x0F12, 0xC000},	// 70002324
	{0x0F12, 0xE59F},	// 70002326
	{0x0F12, 0xFF1C},	// 70002328
	{0x0F12, 0xE12F},	// 7000232A
	{0x0F12, 0x5457},	// 7000232C
	{0x0F12, 0x0000},	// 7000232E
	{0x0F12, 0x4778},	// 70002330
	{0x0F12, 0x46C0},	// 70002332
	{0x0F12, 0xC000},	// 70002334
	{0x0F12, 0xE59F},	// 70002336
	{0x0F12, 0xFF1C},	// 70002338
	{0x0F12, 0xE12F},	// 7000233A
	{0x0F12, 0x5FA3},	// 7000233C
	{0x0F12, 0x0000},	// 7000233E
	{0x0F12, 0x4778},	// 70002340
	{0x0F12, 0x46C0},	// 70002342
	{0x0F12, 0xC000},	// 70002344
	{0x0F12, 0xE59F},	// 70002346
	{0x0F12, 0xFF1C},	// 70002348
	{0x0F12, 0xE12F},	// 7000234A
	{0x0F12, 0x51F9},	// 7000234C
	{0x0F12, 0x0000},	// 7000234E
	{0x0F12, 0x4778},	// 70002350
	{0x0F12, 0x46C0},	// 70002352
	{0x0F12, 0xC000},	// 70002354
	{0x0F12, 0xE59F},	// 70002356
	{0x0F12, 0xFF1C},	// 70002358
	{0x0F12, 0xE12F},	// 7000235A
	{0x0F12, 0x7633},	// 7000235C
	{0x0F12, 0x0000},	// 7000235E
	{0x0F12, 0x4778},	// 70002360
	{0x0F12, 0x46C0},	// 70002362
	{0x0F12, 0xC000},	// 70002364
	{0x0F12, 0xE59F},	// 70002366
	{0x0F12, 0xFF1C},	// 70002368
	{0x0F12, 0xE12F},	// 7000236A
	{0x0F12, 0xA007},	// 7000236C
	{0x0F12, 0x0000},	// 7000236E
	{0x0F12, 0x0360},	// 70002370
	{0x0F12, 0x6F4E},	// 70002372
	{0x0F12, 0x2076},	// 70002374
	{0x0F12, 0x3831},	// 70002376
	{0x0F12, 0x3220},	// 70002378
	{0x0F12, 0x3030},	// 7000237A
	{0x0F12, 0x0038},	// 7000237C
	{0x0F12, 0xE010},	// 7000237E
	{0x0F12, 0x0208},	// 70002380
	{0x0F12, 0x0038},	// 70002382
	// End of Trap and Patch (Last : 70002382h) // Total Size 772 (0x0304)
	{0x0028, 0xD000},
	{0x002A, 0x1000},
	{0x0F12, 0x0001},

	{0x0028, 0x7000},
	{0x002A, 0x0ADC},
	{0x0F12, 0x0AF0},	//#setot_uOnlineClocksDiv40             //2800
	{0x002A, 0x0AE2},
	{0x0F12, 0x222E},	//#setot_usSetRomWaitStateThreshold4KHz //ROM Wait Threshold to 35MHz
	{0x002A, 0x0B94},
	{0x0F12, 0x0500},	//#awbb_GainsInit_0_:R
	{0x0F12, 0x0400},	//#awbb_GainsInit_1_:G
	{0x0F12, 0x0700},	//#awbb_GainsInit_2_:B
	{0x002A, 0x04A0},
	{0x0F12, 0x8000},	//#lt_uLeiInit:AE start
	{0x002A, 0x049A},
	{0x0F12, 0x003B},	//#lt_uMinExp



	//=================================================================================================
	//	Set CIS/APS/Analog
	//=================================================================================================
	// This registers are for FACTORY ONLY. If you change it without prior notification,
	// YOU are RESPONSIBLE for the FAILURE that will happen in the future.
	//=================================================================================================
	{0x0028, 0xD000},
	{0x002A, 0xF106},
	{0x0F12, 0x0001},	// L-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xF206},
	{0x0F12, 0x0001},	// F-OB non sub-sampling: revised by Ana 080128
	{0x002A, 0xC202},
	{0x0F12, 0x0700},	// tgr_coarse_integration_time[15:0]: revised by Ana 080115
	{0x002A, 0xF414},
	{0x0F12, 0x0030},	// aig_adc_sat[7:0] (aig_adc_sat[7:4]+1)*33mV + 690mV
	{0x002A, 0xC204},
	{0x0F12, 0x0100},	// tgr_analogue_code_global[12:0] Analog gain X8
	{0x002A, 0xF402},
	{0x0F12, 0x0092},	// aig_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_offset2[7:0]: revised by Ana 080425
	{0x002A, 0xF700},
	{0x0F12, 0x0040},	// bpradlc_control[7:0]: revised by Ana 080529 // bpradlc_nactive_pedestal[7:5],bpradlc_passthrough[1],bpradlc_bypass[0]
	{0x002A, 0xF708},
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_r[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gr[7:0]: revised by Tg 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_gb[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_b[7:0]: revised by TG 080529
	{0x0F12, 0x0000},	// bpradlc_f_adlc_tune_total[7:0]: revised by TG 080529
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_r[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gr[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_gb[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_b[7:0]: revised by Ana 080425
	{0x0F12, 0x0040},	// bpradlc_line_adlc_tune_total[7:0]: revised by Ana 080425
	{0x0F12, 0x0001},	// bpradlc_adlc_en[7:0]: revised by TG 080529	// bpradlc_f_l_adlc_en[2],bpradlc_f_adlc_en[1],bpradlc_h_adlc_en[0]
	{0x0F12, 0x0015},	// bpradlc_adlc_option[7:0]: revised by TG 080425 // bpradlc_h_adlc_ch_sel[4], // bpradlc_max_data_clip_sel[3:2],bpradlc_f_adlc_max_data_clip_sel[1:0]
	{0x0F12, 0x0001},	// bpradlc_adlc_bpr_en[7:0] adlc_bpr_enable: revised by Ana 080112
	{0x0F12, 0x0040},	// bpradlc_adlc_bpr_thresh[7:0]: 080425
	{0x002A, 0xF48A},
	{0x0F12, 0x0048},	// aig_ld_ptr7[9:0] line OB start addr(04h~48h): revised by Ana 080911
	{0x002A, 0xF10A},
	{0x0F12, 0x008B},	// tgr_h_desc0_addr_end[10:0] line OB end addr(47h~8Bh): revised by Ana 080911 // line OB start - line OB end = 67d
	{0x002A, 0xF900},
	{0x0F12, 0x0067},	// cfpn_config: revised by Ana 080425 // cfpn_ref_gain[9:7],cpfn_ref_lines[6:5],cfpn_hbinning[4],cfpn_output_direct[3],// cfpn_overflow_protect[2],cpfn_passthrough[1],cfpn_bypass[0]
	{0x002A, 0xF406},
	{0x0F12, 0x0092},	// aig_cfpn_ref_offset[7:0] : revised by Ana 080425
	{0x0F12, 0x007F},	// aig_cfpn_ref_offset2[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_cfpn_ref_gain[7:0]: revised by Ana 080425
	{0x0F12, 0x0003},	// aig_ld_ctrl[1:0] aig_ldb_en[1], aig_ld_en[0]
	{0x0F12, 0x0000},	// aig_row_id_ctrl[1:0]
	{0x002A, 0xF442},
	{0x0F12, 0x0000},	// aig_vavg[0]
	{0x0F12, 0x0000},	// aig_havg[0]=1b @H-avg mode: revised by Ana 080116
	{0x002A, 0xF448},
	{0x0F12, 0x0000},	// aig_sl_off[0]
	{0x002A, 0xF454},
	{0x0F12, 0x0001},	//Power save enable
	{0x0F12, 0x0001},	// aig_blst_en[0]
	{0x0F12, 0x0010},	// aig_blst_en_cintr[15:0]
	{0x0F12, 0x0000},	// aig_dshut_en[0]
	{0x002A, 0xF41A},
	{0x0F12, 0x00FF},	// aig_comp_bias[7:0] aig_comp2_bias[7:4], aig_comp1_bias[3:0]: revised by Ana 081013
	{0x0F12, 0x0003},	// aig_pix_bias[3:0]
	{0x002A, 0xF420},
	{0x0F12, 0x0030},	// aig_clp_lvl[7:0]: revised by Ana 080910(refer to 6AA)
	{0x002A, 0xF410},
	{0x0F12, 0x0001},	// aig_clp_sl_ctrl[0]
	{0x0F12, 0x0000},	// aig_cds_test[7:0], aig_cds_test[1]=1b @H-avg mode current save: revised by Ana 080116
	{0x002A, 0xF416},
	{0x0F12, 0x0001},	// aig_rmp_option[7:0]
	{0x002A, 0xF424},
	{0x0F12, 0x0000},	// aig_ref_option[7:0], aig_ref_option[0]=1b @H-avg mode odd cds off: revised by Ana 080116
	{0x002A, 0xF422},
	{0x0F12, 0x0000},	// aig_monit[7:0]
	{0x002A, 0xF41E},
	{0x0F12, 0x0000},	// aig_pd_pix[0]
	{0x002A, 0xF428},
	{0x0F12, 0x0000},	// aig_pd_cp[1:0] aig_pd_ncp[1], aig_pd_cp[0]
	{0x0F12, 0x0000},	// aig_pd_reg_pix[0]
	{0x0F12, 0x0000},	// aig_pd_reg_rg[0]
	{0x002A, 0xF430},
	{0x0F12, 0x0000},	// aig_pd_reg_tgsl[0]
	{0x0F12, 0x0000},	// aig_pd_reg_ntg[0]
	{0x0F12, 0x0008},	// aig_rosc_tune_cp[3:0]: revised by Ana 080418
	{0x0F12, 0x0005},	// aig_rosc_tune_ncp[3:0]
	{0x0F12, 0x000F},	// aig_cp_capa[3:0]
	{0x0F12, 0x0001},	// aig_reg_tune_pix[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_rg[7:0]
	{0x0F12, 0x0040},	// aig_reg_tune_tgsl[7:0]
	{0x0F12, 0x0010},	// aig_reg_tune_ntg[7:0] 0040 -> 0010
	{0x002A, 0xF4D6},
	{0x0F12, 0x00C0},	// aig_v15_tune[7:0]: revised by Ana 081010 // aig_v15_tune[7:4]=7h @sensor only Mode // aig_v15_tune[7:4]=9h @ISP 7.5fps Mode // aig_v15_tune[7:4]=Dh @ISP 15fps Mode
	{0x0F12, 0x0000},	//aig_vreg_sel[7:0] [9]h_test, [1]reg_sw_stby, [0]aig_reg_sel
	{0x002A, 0xF47C},
	{0x0F12, 0x000C},	//aig_ld_ptr0[4:0]
	{0x0F12, 0x0000},	//aig_ld_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF49A},
	{0x0F12, 0x0008},	//aig_sla_ptr0[3:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_sla_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4A2},
	{0x0F12, 0x0008},	//aig_slb_ptr0[7:0]: revised by Ana 080911
	{0x0F12, 0x0000},	//aig_slb_ptr1[8:0]: revised by Ana 081010
	{0x002A, 0xF4B2},
	{0x0F12, 0x0013},	//aig_rxa_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxa_ptr1[9:0]: revised by Ana 081010
	{0x0F12, 0x0013},	//aig_rxb_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0000},	//aig_rxb_ptr1[9:0]: revised by Ana 081010
	{0x002A, 0xF4AA},
	{0x0F12, 0x009B},	//aig_txa_ptr0[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txa_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_txb_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x00FB},	//aig_txb_ptr1[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF474},
	{0x0F12, 0x0017},	//aig_s3_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x005F},	//aig_s3_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0017},	//aig_s4_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x008F},	//aig_s4_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF48C},
	{0x0F12, 0x0017},	//aig_clp_sl_ptr0[6:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x009B},	//aig_clp_sl_ptr1[8:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF4C8},
	{0x0F12, 0x0163},	//aig_off_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x0F12, 0x0193},	//aig_rmp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF490},
	{0x0F12, 0x0191},	//aig_comp_en_ptr0[9:0]: revised by Ana 081016 for CFPN
	{0x002A, 0xF418},
	{0x0F12, 0x0083},	//aig_dbs_option[7:0]: revised by Ana 081010

	//For ESD Check
	{0x0028, 0x7000},
	{0x002A, 0x0132},
	{0x0F12, 0xAAAA},	//#define REG_FWpid (For ESD Check)
	// * note : ESD Check Method.
	//	1. if (7000.0132 != AAAA ) reset // ( S/W reg. check)
	//	2. if (D000.1002 != 0000 ) reset // (internal Exception)

	//Set FPN Gain Input
	{0x002A, 0x1176},
	{0x0F12, 0x0020},	// fpn_GainInput[0]
	{0x0F12, 0x0040},	// fpn_GainInput[1]
	{0x0F12, 0x0080},	// fpn_GainInput[2]
	{0x0F12, 0x0100},	// fpn_GainInput[3]
	{0x0F12, 0x0014},	// fpn_GainOutput[0]
	{0x0F12, 0x000A},	// fpn_GainOutput[1]
	{0x0F12, 0x0008},	// fpn_GainOutput[2]
	{0x0F12, 0x0004},	// fpn_GainOutput[3]

	// Set CFPN Canceller
	{0x002A, 0x116E},
	{0x0F12, 0x0000},	// OvflProtect
	{0x0F12, 0x0000},	// bypassThrough
	{0x0F12, 0x0000},	// bypass
	{0x0F12, 0x0002},	// fpn.CollectMethod	// 0 : Center	1 : Right	2: LEFT
	{0x002A, 0x0AE8},
	{0x0F12, 0x0000},	// setot_bSendFpnToISP = ??

	// Set Forbidden Area
	{0x002A, 0x10EE},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_usRegCount // 0 value --> not use aig table 3
	{0x002A, 0x10F2},
	{0x0F12, 0x0000},	// senHal_SenRegsModes3_pSenModesRegsArray3[0]		//real register 0xD000F45A
	{0x002A, 0x1152},
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[48]    //real register 0xD000F4BA
	{0x0F12, 0x0028},	// senHal_SenRegsModes3_pSenModesRegsArray3[49]    //real register 0xD000F4BC
	{0x0F12, 0x0030},	// senHal_SenRegsModes3_pSenModesRegsArray3[50]    //real register 0xD000F4BE
	{0x002A, 0x1148},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[43]    //real register 0xD000F4B0
	{0x002A, 0x1144},
	{0x0F12, 0x00FB},	// senHal_SenRegsModes3_pSenModesRegsArray3[41]    //real register 0xD000F4AC
	{0x002A, 0x1150},
	{0x0F12, 0x0148},	// senHal_SenRegsModes3_pSenModesRegsArray3[47]    //real register 0xD000F4B8

	// Set AE Target
	{0x002A, 0x0F4C},
	{0x0F12, 0x003D},	// #TVAR_ae_BrAve

	//=================================================================================================
	//	Set Frame Rate
	//=================================================================================================
	{0x002A, 0x0484},
	{0x0F12, 0x411A},	// #lt_uMaxExp1 	// 33ms
	{0x0F12, 0x0000},
	{0x0F12, 0x86A0},	// #lt_uMaxExp2 	// 200ms
	{0x0F12, 0x0001},
	{0x0F12, 0x8214},	// #lt_uCapMaxExp1	// 66ms
	{0x0F12, 0x0000},
	{0x0F12, 0xD2F0},	// #lt_uCapMaxExp2	// 108ms
	{0x0F12, 0x0000},
	{0x0F12, 0x04C0},	// #lt_uMaxAnGain1	// x4.75
	{0x0F12, 0x0700},	// #lt_uMaxAnGain2	// x7.0
	{0x0F12, 0x0100},	// #lt_uMaxDigGain
	{0x002A, 0x0F52},
	{0x0F12, 0x0007},	// #ae_StatMode

	//=================================================================================================
	//	Set Flicker  Current Flicker = 7000.164A //Mon_AFC_usFlExpQunat		0001:Off 1045:60Hz	1388:50Hz
	//=================================================================================================

	//1. Auto Flicker 60Hz Start
	{0x002A, 0x0B2E},
	{0x0F12, 0x0001},	// #AFC_Default60Hz 	1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	{0x002A, 0x03F8},
	{0x0F12, 0x007F},	// #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////2. Auto Flicker 50Hz Start
	//WRITE	70000B2E	0000	// #AFC_Default60Hz		1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
	//WRITE 700003F8	007F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F

	////3. Manual Flicker 60Hz
	//WRITE 700003F8	005F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F
	//WRITE 700003D4	0002  // #REG_SF_USER_FlickerQuant	 //Flicker 50Hz:0001/60Hz:0002/off:0000
	//WRITE 700003D6	0001  // #REG_SF_USER_FlickerQuantChanged //if change 0001 write

	////4. Manual Flicker 50Hz
	//WRITE 700003F8	005F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F
	//WRITE 700003D4	0001  // #REG_SF_USER_FlickerQuant	 //Flicker 50Hz:0001/60Hz:0002/off:0000
	//WRITE 700003D6	0001  // #REG_SF_USER_FlickerQuantChanged //if change 0001 write

	////5. Flicker Off
	//WRITE 700003F8	005F  // #REG_TC_DBG_AutoAlgEnBits	 //default : 007F
	//WRITE 700003D4	0000  // #REG_SF_USER_FlickerQuant	 //Flicker 50Hz:0001/60Hz:0002/off:0000
	//WRITE 700003D6	0001  // #REG_SF_USER_FlickerQuantChanged //if change 0001 write


	//WRITE 70000B36	0001  // #AFC_ManualQuant


	// added for test
	//WRITE #senHal_uMinColsAddAnalogBin       	 	0048		//Subsampling시 1-H min 값  1768로 조절하기 위한 Register
	//WRITE #setot_uOnlineClocksDiv40		 	  			0A28 		//2800
	//WRITE #setot_usSetRomWaitStateThreshold4KHz	222E 		//ROM Wait Threshold to 35MHz



	//=================================================================================================
	//	Set PLL
	//=================================================================================================
	// Input CLK
	{0x002A, 0x01B8},
	{0x0F12, 0x5DC0},	//#REG_TC_IPRM_InClockLSBs	//24MHz
	{0x0F12, 0x0000},	//#REG_TC_IPRM_InClockMSBs

	// Target Sys CLK
	{0x002A, 0x01CC},
	{0x0F12, 0x222E},	// #REG_TC_IPRM_OpClk4KHz_0 			//35Mhz

	// Target PVI CLK
	{0x0F12, 0x444C},	// #REG_TC_IPRM_MinOutRate4KHz_0	//70Mhz
	{0x0F12, 0x446C},	// #REG_TC_IPRM_MaxOutRate4KHz_0

	// PLL ETC
	{0x002A, 0x01C6},
	{0x0F12, 0x0003},	//#REG_TC_IPRM_UseNPviClocks
	{0x0F12, 0x0000},	//#REG_TC_IPRM_UseNMipiClocks
	{0x002A, 0x01C0},
	{0x0F12, 0x0000},	//#EG_TC_IPRM_ValidVActiveLow
	{0x002A, 0x01DE},
	{0x0F12, 0x0001},	//#REG_TC_IPRM_UseRegsAPI
	{0x0F12, 0x0001},	//#REG_TC_IPRM_InitParamsUpdated
	{0x002A, 0x1084},
	{0x0F12, 0x0001},	//#senHal_bUseAnalogBinning //Avg S.S
	{0x002A, 0x10B6},
	{0x0F12, 0x0048},	//#senHal_uMinColsAddAnalogBin



	//=================================================================================================
	//	Set Preview Config
	//=================================================================================================
	{0x002A, 0x0242},
	{0x0F12, 0x0280}, //0x0280, //0x0140(320)->640, //090416 800 -> 320, 0x0320,	//0280  // #REG_0TC_PCFG_usWidth						// 640 -> 800 _20081209
	{0x0F12, 0x01E0}, //0x01e0, //0x00f0(240)->480, //090416 600 -> 240, 0x0258,	//01e0  // #REG_0TC_PCFG_usHeight						// 480 -> 600 _20081209
	{0x0F12, 0x0005},	// #REG_0TC_PCFG_Format 						// YUV output
	{0x0F12, 0x446C},	// #REG_0TC_PCFG_usMaxOut4KHzRate
	{0x0F12, 0x444C},	// #REG_0TC_PCFG_usMinOut4KHzRate
	{0x0F12, 0x0052},	// #REG_0TC_PCFG_PVIMask   (x2: PCLK rising x0: PCLK Falling)
//	{0x0F12, 0x0050},	// #REG_0TC_PCFG_PVIMask   (x2: PCLK rising x0: PCLK Falling)
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_uClockInd
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_usFrTimeType
	{0x0F12, 0x0000},	// #REG_0TC_PCFG_FrRateQualityType
	{0x0F12, 0x03E8},	// #REG_0TC_PCFG_usMaxFrTimeMsecMult10 // 10fps
	{0x0F12, 0x014D},	// #REG_0TC_PCFG_usMinFrTimeMsecMult10 // 30fps
	{0x002A, 0x0262},
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uPrevMirror -- X/Y flip 20090112
	{0x0F12, 0x0000},	//0x0003,	//#REG_0TC_PCFG_uCaptureMirror -- X/Y flip 20090112
	//WRITE 70000262 000F  //#REG_0TC_PCFG_uPrevMirror
	//WRITE 70000264 000F  //#REG_0TC_PCFG_uCaptureMirror


	// Apply preview config
	{0x002A, 0x021C},
	{0x0F12, 0x0000},	//#REG_TC_GP_ActivePrevConfig	  //preview config0
	{0x002A, 0x0220},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevOpenAfterChange  //config 변경적용
	{0x002A, 0x01F8},
	{0x0F12, 0x0001},	//#REG_TC_GP_NewConfigSync
	{0x002A, 0x021E},
	{0x0F12, 0x0001},	//#REG_TC_GP_PrevConfigChanged



	//=================================================================================================
	//	Set Capture Config
	//=================================================================================================
	// For 1600x1202 capture
	//WRITE 70000202	0640	// #REG_TC_GP_CapReqInputWidth
	//WRITE 70000204	04C0	// #REG_TC_GP_CapReqInputHeight			// Vsize = 1216
	//WRITE 70000206	0000	// #REG_TC_GP_CapInputWidthOfs
	//WRITE 70000208	0000	// #REG_TC_GP_CapInputHeightOfs
	//
	//WRITE 70000212	0640	// #REG_TC_GP_CapZoomReqInputWidth
	//WRITE 70000214	04C0	// #REG_TC_GP_CapZoomReqInputHeight	// Vsize = 1216
	//WRITE 70000216	0000	// #REG_TC_GP_CapZoomReqInputWidthOfs
	//WRITE 70000218	0000	// #REG_TC_GP_CapZoomReqInputHeightOfs
	//WRITE 7000021A	0001	// #REG_TC_GP_InputsChangeRequest
	//WRITE 700001F8	0001	// #REG_TC_GP_NewConfigSync
	//
	{0x002A, 0x0302},
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uCaptureMode
	{0x0F12, 0x0640},	//#REG_0TC_CCFG_usWidth
	{0x0F12, 0x04B0},	//#REG_0TC_CCFG_usHeight						// Vsize = 1216
	{0x0F12, 0x0005},	//#REG_0TC_CCFG_Format	   //5:YUV,9:JPEG
	{0x0F12, 0x446C},	//#REG_0TC_CCFG_usMaxOut4KHzRate
	{0x0F12, 0x444C},	//#REG_0TC_CCFG_usMinOut4KHzRate
	{0x0F12, 0x0052},	//#REG_0TC_CCFG_PVIMask (x2: PCLK rising	x0: PCLK Falling)
//	{0x0F12, 0x0050},	//#REG_0TC_CCFG_PVIMask (x2: PCLK rising	x0: PCLK Falling)
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_uClockInd
	{0x0F12, 0x0000},	//#REG_0TC_CCFG_usFrTimeType
	{0x0F12, 0x0002},	//#REG_0TC_CCFG_FrRateQualityType
	{0x0F12, 0x07D0},	//#REG_0TC_CCFG_usMaxFrTimeMsecMult10 // 5fps
	{0x0F12, 0x0535},	//#REG_0TC_CCFG_usMinFrTimeMsecMult10 // 7.5fps



	//=================================================================================================
	//	Set Periodic Mismatch (Use default value)
	//=================================================================================================
	//WRITE 70000780	0000 //FFF0  // #msm_uOffsetNoBin[0][0]
	//WRITE 70000782	0000 //FFF0  // #msm_uOffsetNoBin[0][1]
	//WRITE 70000784	0000 //FFEE  // #msm_uOffsetNoBin[1][0]
	//WRITE 70000786	0000 //FFE0  // #msm_uOffsetNoBin[1][1]
	//WRITE 70000788	0000 //FFF0  // #msm_uOffsetNoBin[2][0]
	//WRITE 7000078A	0000 //FFF0  // #msm_uOffsetNoBin[2][1]
	//WRITE 7000078C	0000 //FFEE  // #msm_uOffsetNoBin[3][0]
	//WRITE 7000078E	0000 //FFE0  // #msm_uOffsetNoBin[3][1]
	//WRITE 70000798  0000 //FFE0  // #msm_uOffsetBin[0][0]
	//WRITE 7000079A  0000 //FFEE  // #msm_uOffsetBin[0][1]
	//WRITE 7000079C  0000 //FFF0  // #msm_uOffsetBin[1][0]
	//WRITE 7000079E  0000 //FFFB  // #msm_uOffsetBin[1][1]



	//=================================================================================================
	//	Set AE Weights
	//=================================================================================================
	{0x002A, 0x0F5A},
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_0_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_1_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_2_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_3_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_4_
	{0x0F12, 0x0000},	//0201	// #ae_WeightTbl_16_5_
	{0x0F12, 0x0000},	//0102	// #ae_WeightTbl_16_6_
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_7_
	{0x0F12, 0x0101},	//0101	// #ae_WeightTbl_16_8_
	{0x0F12, 0x0101},	//0202	// #ae_WeightTbl_16_9_
	{0x0F12, 0x0101},	//0202	// #ae_WeightTbl_16_10
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_11
	{0x0F12, 0x0201},	//0101	// #ae_WeightTbl_16_12
	{0x0F12, 0x0202},	//0402	// #ae_WeightTbl_16_13
	{0x0F12, 0x0102},	//0204	// #ae_WeightTbl_16_14
	{0x0F12, 0x0000},	//0101	// #ae_WeightTbl_16_15
	{0x0F12, 0x0201},	//0201	// #ae_WeightTbl_16_16
	{0x0F12, 0x0505},	//0504	// #ae_WeightTbl_16_17
	{0x0F12, 0x0102},	//0405	// #ae_WeightTbl_16_18
	{0x0F12, 0x0000},	//0102	// #ae_WeightTbl_16_19
	{0x0F12, 0x0201},	//0501	// #ae_WeightTbl_16_20
	{0x0F12, 0x0505},	//0505	// #ae_WeightTbl_16_21
	{0x0F12, 0x0102},	//0505	// #ae_WeightTbl_16_22
	{0x0F12, 0x0000},	//0105	// #ae_WeightTbl_16_23
	{0x0F12, 0x0201},	//0503	// #ae_WeightTbl_16_24
	{0x0F12, 0x0202},	//0505	// #ae_WeightTbl_16_25
	{0x0F12, 0x0102},	//0505	// #ae_WeightTbl_16_26
	{0x0F12, 0x0000},	//0305	// #ae_WeightTbl_16_27
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_28
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_29
	{0x0F12, 0x0101},	//0303	// #ae_WeightTbl_16_30
	{0x0F12, 0x0000},	//0303	// #ae_WeightTbl_16_31



	//=================================================================================================
	//	Set GAS & CCM White Point
	//=================================================================================================
	// param_start	     2TVAR_ash_AwbAshCord
	{0x002A, 0x0704},
	{0x0F12, 0x0090},	//000E	// #TVAR_ash_AwbAshCord_0_
	{0x0F12, 0x00B3},	//0012	// #TVAR_ash_AwbAshCord_1_
	{0x0F12, 0x00C3},	//0019	// #TVAR_ash_AwbAshCord_2_
	{0x0F12, 0x00E5},	//0034	// #TVAR_ash_AwbAshCord_3_
	{0x0F12, 0x0116},	//0044	// #TVAR_ash_AwbAshCord_4_
	{0x0F12, 0x0198},	//0052	// #TVAR_ash_AwbAshCord_5_
	{0x0F12, 0x0200},	//0058	// #TVAR_ash_AwbAshCord_6_

	// param_start	wbt_AwbCcmCord
	{0x002A, 0x06F2},
	{0x0F12, 0x00B3},	//0016	// #SARR_AwbCcmCord_0_	Hor
	{0x0F12, 0x00E5},	//001E	// #SARR_AwbCcmCord_1_	IncaA
	{0x0F12, 0x0100},	//0024	// #SARR_AwbCcmCord_2_	WW
	{0x0F12, 0x0116},	//0038	// #SARR_AwbCcmCord_3_	CW
	{0x0F12, 0x0150},	//004C	// #SARR_AwbCcmCord_4_	D50
	{0x0F12, 0x0198},	//0058	// #SARR_AwbCcmCord_5_	D65

	// Set Target Brightness Control
	{0x002A, 0x103E},
	{0x0F12, 0x0000},	// #SARR_IllumType_0_
	{0x0F12, 0x0009},	// #SARR_IllumType_1_
	{0x0F12, 0x0018},	// #SARR_IllumType_2_
	{0x0F12, 0x0032},	// #SARR_IllumType_3_
	{0x0F12, 0x004A},	// #SARR_IllumType_4_
	{0x0F12, 0x0051},	// #SARR_IllumType_5_
	{0x0F12, 0x0056},	// #SARR_IllumType_6_
	{0x0F12, 0x010C},	// #SARe_2_R_IllumTypeF_0_
	{0x0F12, 0x010C},	// #SARe_3_R_IllumTypeF_1_
	{0x0F12, 0x0109},	// #SARe_4_R_IllumTypeF_2_
	{0x0F12, 0x0105},	// #SARe_5_R_IllumTypeF_3_
	{0x0F12, 0x0102},	// #SARe_6_R_IllumTypeF_4_
	{0x0F12, 0x00FB},	// #SARR_IllumTypeF_5_
	{0x0F12, 0x00F8},	// #SARR_IllumTypeF_6_

	// Indoor GAS Alpha
	{0x002A, 0x0712},
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[0]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[1]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[2]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[3]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[4]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[5]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[6]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[7]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[8]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[9]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[10]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[11]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[12]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[13]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[14]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[15]
	{0x0F12, 0x00E0},	// #TVAR_ash_GASAlpha[16]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[17]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[18]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[19]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[20]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[21]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[22]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[23]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[24]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[25]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[26]
	{0x0F12, 0x0100},	// #TVAR_ash_GASAlpha[27]

	// Outdoor GAS Alpha
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_0_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_1_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_2_
	{0x0F12, 0x0100},	// #TVAR_ash_GASOutdoorAlpha_3_

	// GAS LUT Start Address
	{0x002A, 0x0754},
	{0x0F12, 0x2388},	//#TVAR_ash_pGAS
	{0x0F12, 0x7000},	//#TVAR_ash_pGAS

	// GAS LUT
	//							//1030	//1027
	{0x002A, 0x2388},
	{0x0F12, 0x01C9},	//#TVAR_ash_pGAS[0]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[1]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[2]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[3]
	{0x0F12, 0x0105},	//#TVAR_ash_pGAS[4]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[5]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[6]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[7]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[8]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[9]
	{0x0F12, 0x0160},	//#TVAR_ash_pGAS[10]
	{0x0F12, 0x0189},	//#TVAR_ash_pGAS[11]
	{0x0F12, 0x01D0},	//#TVAR_ash_pGAS[12]
	{0x0F12, 0x0193},	//#TVAR_ash_pGAS[13]
	{0x0F12, 0x016A},	//#TVAR_ash_pGAS[14]
	{0x0F12, 0x0126},	//#TVAR_ash_pGAS[15]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[16]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[17]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[18]
	{0x0F12, 0x009B},	//#TVAR_ash_pGAS[19]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[20]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[21]
	{0x0F12, 0x00EF},	//#TVAR_ash_pGAS[22]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[23]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[24]
	{0x0F12, 0x019C},	//#TVAR_ash_pGAS[25]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[26]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[27]
	{0x0F12, 0x00E2},	//#TVAR_ash_pGAS[28]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[29]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[30]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[31]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[32]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[33]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[34]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[35]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[36]
	{0x0F12, 0x0133},	//#TVAR_ash_pGAS[37]
	{0x0F12, 0x016E},	//#TVAR_ash_pGAS[38]
	{0x0F12, 0x0140},	//#TVAR_ash_pGAS[39]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[40]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[41]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[42]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[43]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[44]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[45]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[46]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[47]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[48]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[49]
	{0x0F12, 0x010D},	//#TVAR_ash_pGAS[50]
	{0x0F12, 0x0157},	//#TVAR_ash_pGAS[51]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[52]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[53]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[54]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[55]
	{0x0F12, 0x002F},	//#TVAR_ash_pGAS[56]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[57]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[58]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[59]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[60]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[61]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[62]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[63]
	{0x0F12, 0x0142},	//#TVAR_ash_pGAS[64]
	{0x0F12, 0x011A},	//#TVAR_ash_pGAS[65]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[66]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[67]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[68]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[69]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[70]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[71]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[72]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[73]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[74]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[75]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[76]
	{0x0F12, 0x013B},	//#TVAR_ash_pGAS[77]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[78]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[79]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[80]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[81]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[82]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[83]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[84]
	{0x0F12, 0x0014},	//#TVAR_ash_pGAS[85]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[86]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[87]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[88]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[89]
	{0x0F12, 0x013F},	//#TVAR_ash_pGAS[90]
	{0x0F12, 0x012C},	//#TVAR_ash_pGAS[91]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[92]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[93]
	{0x0F12, 0x0067},	//#TVAR_ash_pGAS[94]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[95]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[96]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[97]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[98]
	{0x0F12, 0x004B},	//#TVAR_ash_pGAS[99]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[100]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[101]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[102]
	{0x0F12, 0x0150},	//#TVAR_ash_pGAS[103]
	{0x0F12, 0x0149},	//#TVAR_ash_pGAS[104]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[105]
	{0x0F12, 0x00C5},	//#TVAR_ash_pGAS[106]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[107]
	{0x0F12, 0x0061},	//#TVAR_ash_pGAS[108]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[109]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[110]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[111]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[112]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[113]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[114]
	{0x0F12, 0x012E},	//#TVAR_ash_pGAS[115]
	{0x0F12, 0x016D},	//#TVAR_ash_pGAS[116]
	{0x0F12, 0x0167},	//#TVAR_ash_pGAS[117]
	{0x0F12, 0x013A},	//#TVAR_ash_pGAS[118]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[119]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[120]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[121]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[122]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[123]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[124]
	{0x0F12, 0x00A6},	//#TVAR_ash_pGAS[125]
	{0x0F12, 0x00D8},	//#TVAR_ash_pGAS[126]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[127]
	{0x0F12, 0x0154},	//#TVAR_ash_pGAS[128]
	{0x0F12, 0x0186},	//#TVAR_ash_pGAS[129]
	{0x0F12, 0x018F},	//#TVAR_ash_pGAS[130]
	{0x0F12, 0x0161},	//#TVAR_ash_pGAS[131]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[132]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[133]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[134]
	{0x0F12, 0x00B4},	//#TVAR_ash_pGAS[135]
	{0x0F12, 0x00B1},	//#TVAR_ash_pGAS[136]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[137]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[138]
	{0x0F12, 0x0112},	//#TVAR_ash_pGAS[139]
	{0x0F12, 0x014F},	//#TVAR_ash_pGAS[140]
	{0x0F12, 0x0185},	//#TVAR_ash_pGAS[141]
	{0x0F12, 0x01A1},	//#TVAR_ash_pGAS[142]
	{0x0F12, 0x0168},	//#TVAR_ash_pGAS[143]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[144]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[145]
	{0x0F12, 0x00DA},	//#TVAR_ash_pGAS[146]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[147]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[148]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[149]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[150]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[151]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[152]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[153]
	{0x0F12, 0x0125},	//#TVAR_ash_pGAS[154]
	{0x0F12, 0x0163},	//#TVAR_ash_pGAS[155]
	{0x0F12, 0x0131},	//#TVAR_ash_pGAS[156]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[157]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[158]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[159]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[160]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[161]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[162]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[163]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[164]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[165]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[166]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[167]
	{0x0F12, 0x012F},	//#TVAR_ash_pGAS[168]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[169]
	{0x0F12, 0x00D2},	//#TVAR_ash_pGAS[170]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[171]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[172]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[173]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[174]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[175]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[176]
	{0x0F12, 0x0059},	//#TVAR_ash_pGAS[177]
	{0x0F12, 0x0078},	//#TVAR_ash_pGAS[178]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[179]
	{0x0F12, 0x00DC},	//#TVAR_ash_pGAS[180]
	{0x0F12, 0x010C},	//#TVAR_ash_pGAS[181]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[182]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[183]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[184]
	{0x0F12, 0x0052},	//#TVAR_ash_pGAS[185]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[186]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[187]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[188]
	{0x0F12, 0x0025},	//#TVAR_ash_pGAS[189]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[190]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[191]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[192]
	{0x0F12, 0x00C2},	//#TVAR_ash_pGAS[193]
	{0x0F12, 0x00FD},	//#TVAR_ash_pGAS[194]
	{0x0F12, 0x00DD},	//#TVAR_ash_pGAS[195]
	{0x0F12, 0x00A0},	//#TVAR_ash_pGAS[196]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[197]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[198]
	{0x0F12, 0x001E},	//#TVAR_ash_pGAS[199]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[200]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[201]
	{0x0F12, 0x0010},	//#TVAR_ash_pGAS[202]
	{0x0F12, 0x0026},	//#TVAR_ash_pGAS[203]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[204]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[205]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[206]
	{0x0F12, 0x00EC},	//#TVAR_ash_pGAS[207]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[208]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[209]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[210]
	{0x0F12, 0x0036},	//#TVAR_ash_pGAS[211]
	{0x0F12, 0x0017},	//#TVAR_ash_pGAS[212]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[213]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[214]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[215]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[216]
	{0x0F12, 0x0043},	//#TVAR_ash_pGAS[217]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[218]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[219]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[220]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[221]
	{0x0F12, 0x009A},	//#TVAR_ash_pGAS[222]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[223]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[224]
	{0x0F12, 0x001B},	//#TVAR_ash_pGAS[225]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[226]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[227]
	{0x0F12, 0x000F},	//#TVAR_ash_pGAS[228]
	{0x0F12, 0x0027},	//#TVAR_ash_pGAS[229]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[230]
	{0x0F12, 0x007A},	//#TVAR_ash_pGAS[231]
	{0x0F12, 0x00B3},	//#TVAR_ash_pGAS[232]
	{0x0F12, 0x00F1},	//#TVAR_ash_pGAS[233]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[234]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[235]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[236]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[237]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[238]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[239]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[240]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[241]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[242]
	{0x0F12, 0x005E},	//#TVAR_ash_pGAS[243]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[244]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[245]
	{0x0F12, 0x00FE},	//#TVAR_ash_pGAS[246]
	{0x0F12, 0x00FA},	//#TVAR_ash_pGAS[247]
	{0x0F12, 0x00C3},	//#TVAR_ash_pGAS[248]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[249]
	{0x0F12, 0x0064},	//#TVAR_ash_pGAS[250]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[251]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[252]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[253]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[254]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[255]
	{0x0F12, 0x007D},	//#TVAR_ash_pGAS[256]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[257]
	{0x0F12, 0x00DF},	//#TVAR_ash_pGAS[258]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[259]
	{0x0F12, 0x0110},	//#TVAR_ash_pGAS[260]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[261]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[262]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[263]
	{0x0F12, 0x006E},	//#TVAR_ash_pGAS[264]
	{0x0F12, 0x0060},	//#TVAR_ash_pGAS[265]
	{0x0F12, 0x005F},	//#TVAR_ash_pGAS[266]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[267]
	{0x0F12, 0x0083},	//#TVAR_ash_pGAS[268]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[269]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[270]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[271]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[272]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[273]
	{0x0F12, 0x0104},	//#TVAR_ash_pGAS[274]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[275]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[276]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[277]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[278]
	{0x0F12, 0x008C},	//#TVAR_ash_pGAS[279]
	{0x0F12, 0x0098},	//#TVAR_ash_pGAS[280]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[281]
	{0x0F12, 0x00D1},	//#TVAR_ash_pGAS[282]
	{0x0F12, 0x00FF},	//#TVAR_ash_pGAS[283]
	{0x0F12, 0x0127},	//#TVAR_ash_pGAS[284]
	{0x0F12, 0x0146},	//#TVAR_ash_pGAS[285]
	{0x0F12, 0x0166},	//#TVAR_ash_pGAS[286]
	{0x0F12, 0x012B},	//#TVAR_ash_pGAS[287]
	{0x0F12, 0x0100},	//#TVAR_ash_pGAS[288]
	{0x0F12, 0x00D9},	//#TVAR_ash_pGAS[289]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[290]
	{0x0F12, 0x00AA},	//#TVAR_ash_pGAS[291]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[292]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[293]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[294]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[295]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[296]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[297]
	{0x0F12, 0x0180},	//#TVAR_ash_pGAS[298]
	{0x0F12, 0x0132},	//#TVAR_ash_pGAS[299]
	{0x0F12, 0x0102},	//#TVAR_ash_pGAS[300]
	{0x0F12, 0x00D0},	//#TVAR_ash_pGAS[301]
	{0x0F12, 0x00A5},	//#TVAR_ash_pGAS[302]
	{0x0F12, 0x0086},	//#TVAR_ash_pGAS[303]
	{0x0F12, 0x0076},	//#TVAR_ash_pGAS[304]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[305]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[306]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[307]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[308]
	{0x0F12, 0x00EE},	//#TVAR_ash_pGAS[309]
	{0x0F12, 0x011E},	//#TVAR_ash_pGAS[310]
	{0x0F12, 0x014C},	//#TVAR_ash_pGAS[311]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[312]
	{0x0F12, 0x00D7},	//#TVAR_ash_pGAS[313]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[314]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[315]
	{0x0F12, 0x0057},	//#TVAR_ash_pGAS[316]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[317]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[318]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[319]
	{0x0F12, 0x0066},	//#TVAR_ash_pGAS[320]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[321]
	{0x0F12, 0x00BC},	//#TVAR_ash_pGAS[322]
	{0x0F12, 0x00F4},	//#TVAR_ash_pGAS[323]
	{0x0F12, 0x0129},	//#TVAR_ash_pGAS[324]
	{0x0F12, 0x00F6},	//#TVAR_ash_pGAS[325]
	{0x0F12, 0x00B9},	//#TVAR_ash_pGAS[326]
	{0x0F12, 0x0081},	//#TVAR_ash_pGAS[327]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[328]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[329]
	{0x0F12, 0x0024},	//#TVAR_ash_pGAS[330]
	{0x0F12, 0x0020},	//#TVAR_ash_pGAS[331]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[332]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[333]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[334]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[335]
	{0x0F12, 0x00D5},	//#TVAR_ash_pGAS[336]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[337]
	{0x0F12, 0x00E3},	//#TVAR_ash_pGAS[338]
	{0x0F12, 0x00A3},	//#TVAR_ash_pGAS[339]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[340]
	{0x0F12, 0x0041},	//#TVAR_ash_pGAS[341]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[342]
	{0x0F12, 0x000D},	//#TVAR_ash_pGAS[343]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[344]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[345]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[346]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[347]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[348]
	{0x0F12, 0x00BA},	//#TVAR_ash_pGAS[349]
	{0x0F12, 0x00F7},	//#TVAR_ash_pGAS[350]
	{0x0F12, 0x00DE},	//#TVAR_ash_pGAS[351]
	{0x0F12, 0x009D},	//#TVAR_ash_pGAS[352]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[353]
	{0x0F12, 0x003A},	//#TVAR_ash_pGAS[354]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[355]
	{0x0F12, 0x0005},	//#TVAR_ash_pGAS[356]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[357]
	{0x0F12, 0x0009},	//#TVAR_ash_pGAS[358]
	{0x0F12, 0x0021},	//#TVAR_ash_pGAS[359]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[360]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[361]
	{0x0F12, 0x00AF},	//#TVAR_ash_pGAS[362]
	{0x0F12, 0x00ED},	//#TVAR_ash_pGAS[363]
	{0x0F12, 0x00E0},	//#TVAR_ash_pGAS[364]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[365]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[366]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[367]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[368]
	{0x0F12, 0x0008},	//#TVAR_ash_pGAS[369]
	{0x0F12, 0x0002},	//#TVAR_ash_pGAS[370]
	{0x0F12, 0x000C},	//#TVAR_ash_pGAS[371]
	{0x0F12, 0x0023},	//#TVAR_ash_pGAS[372]
	{0x0F12, 0x0046},	//#TVAR_ash_pGAS[373]
	{0x0F12, 0x0073},	//#TVAR_ash_pGAS[374]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[375]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[376]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[377]
	{0x0F12, 0x00B0},	//#TVAR_ash_pGAS[378]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[379]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[380]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[381]
	{0x0F12, 0x0019},	//#TVAR_ash_pGAS[382]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[383]
	{0x0F12, 0x001C},	//#TVAR_ash_pGAS[384]
	{0x0F12, 0x0032},	//#TVAR_ash_pGAS[385]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[386]
	{0x0F12, 0x007F},	//#TVAR_ash_pGAS[387]
	{0x0F12, 0x00B7},	//#TVAR_ash_pGAS[388]
	{0x0F12, 0x00F2},	//#TVAR_ash_pGAS[389]
	{0x0F12, 0x0106},	//#TVAR_ash_pGAS[390]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[391]
	{0x0F12, 0x0096},	//#TVAR_ash_pGAS[392]
	{0x0F12, 0x006C},	//#TVAR_ash_pGAS[393]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[394]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[395]
	{0x0F12, 0x0033},	//#TVAR_ash_pGAS[396]
	{0x0F12, 0x003C},	//#TVAR_ash_pGAS[397]
	{0x0F12, 0x0051},	//#TVAR_ash_pGAS[398]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[399]
	{0x0F12, 0x0099},	//#TVAR_ash_pGAS[400]
	{0x0F12, 0x00CC},	//#TVAR_ash_pGAS[401]
	{0x0F12, 0x0101},	//#TVAR_ash_pGAS[402]
	{0x0F12, 0x0120},	//#TVAR_ash_pGAS[403]
	{0x0F12, 0x00F0},	//#TVAR_ash_pGAS[404]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[405]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[406]
	{0x0F12, 0x0074},	//#TVAR_ash_pGAS[407]
	{0x0F12, 0x0062},	//#TVAR_ash_pGAS[408]
	{0x0F12, 0x005C},	//#TVAR_ash_pGAS[409]
	{0x0F12, 0x0063},	//#TVAR_ash_pGAS[410]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[411]
	{0x0F12, 0x0095},	//#TVAR_ash_pGAS[412]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[413]
	{0x0F12, 0x00EA},	//#TVAR_ash_pGAS[414]
	{0x0F12, 0x0116},	//#TVAR_ash_pGAS[415]
	{0x0F12, 0x0141},	//#TVAR_ash_pGAS[416]
	{0x0F12, 0x0115},	//#TVAR_ash_pGAS[417]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[418]
	{0x0F12, 0x00BF},	//#TVAR_ash_pGAS[419]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[420]
	{0x0F12, 0x0091},	//#TVAR_ash_pGAS[421]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[422]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[423]
	{0x0F12, 0x00A4},	//#TVAR_ash_pGAS[424]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[425]
	{0x0F12, 0x00E9},	//#TVAR_ash_pGAS[426]
	{0x0F12, 0x010E},	//#TVAR_ash_pGAS[427]
	{0x0F12, 0x0130},	//#TVAR_ash_pGAS[428]
	{0x0F12, 0x011C},	//#TVAR_ash_pGAS[429]
	{0x0F12, 0x00EB},	//#TVAR_ash_pGAS[430]
	{0x0F12, 0x00C6},	//#TVAR_ash_pGAS[431]
	{0x0F12, 0x00A7},	//#TVAR_ash_pGAS[432]
	{0x0F12, 0x0090},	//#TVAR_ash_pGAS[433]
	{0x0F12, 0x0085},	//#TVAR_ash_pGAS[434]
	{0x0F12, 0x0082},	//#TVAR_ash_pGAS[435]
	{0x0F12, 0x008B},	//#TVAR_ash_pGAS[436]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[437]
	{0x0F12, 0x00C0},	//#TVAR_ash_pGAS[438]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[439]
	{0x0F12, 0x0103},	//#TVAR_ash_pGAS[440]
	{0x0F12, 0x0135},	//#TVAR_ash_pGAS[441]
	{0x0F12, 0x00F3},	//#TVAR_ash_pGAS[442]
	{0x0F12, 0x00CB},	//#TVAR_ash_pGAS[443]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[444]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[445]
	{0x0F12, 0x0069},	//#TVAR_ash_pGAS[446]
	{0x0F12, 0x005D},	//#TVAR_ash_pGAS[447]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[448]
	{0x0F12, 0x0065},	//#TVAR_ash_pGAS[449]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[450]
	{0x0F12, 0x0094},	//#TVAR_ash_pGAS[451]
	{0x0F12, 0x00BD},	//#TVAR_ash_pGAS[452]
	{0x0F12, 0x00E7},	//#TVAR_ash_pGAS[453]
	{0x0F12, 0x0108},	//#TVAR_ash_pGAS[454]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[455]
	{0x0F12, 0x00A1},	//#TVAR_ash_pGAS[456]
	{0x0F12, 0x0077},	//#TVAR_ash_pGAS[457]
	{0x0F12, 0x0056},	//#TVAR_ash_pGAS[458]
	{0x0F12, 0x0042},	//#TVAR_ash_pGAS[459]
	{0x0F12, 0x0037},	//#TVAR_ash_pGAS[460]
	{0x0F12, 0x0035},	//#TVAR_ash_pGAS[461]
	{0x0F12, 0x003D},	//#TVAR_ash_pGAS[462]
	{0x0F12, 0x004F},	//#TVAR_ash_pGAS[463]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[464]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[465]
	{0x0F12, 0x00C1},	//#TVAR_ash_pGAS[466]
	{0x0F12, 0x00E5},	//#TVAR_ash_pGAS[467]
	{0x0F12, 0x00B5},	//#TVAR_ash_pGAS[468]
	{0x0F12, 0x0088},	//#TVAR_ash_pGAS[469]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[470]
	{0x0F12, 0x003E},	//#TVAR_ash_pGAS[471]
	{0x0F12, 0x002A},	//#TVAR_ash_pGAS[472]
	{0x0F12, 0x001D},	//#TVAR_ash_pGAS[473]
	{0x0F12, 0x001A},	//#TVAR_ash_pGAS[474]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[475]
	{0x0F12, 0x0034},	//#TVAR_ash_pGAS[476]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[477]
	{0x0F12, 0x0072},	//#TVAR_ash_pGAS[478]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[479]
	{0x0F12, 0x00CE},	//#TVAR_ash_pGAS[480]
	{0x0F12, 0x00A2},	//#TVAR_ash_pGAS[481]
	{0x0F12, 0x0075},	//#TVAR_ash_pGAS[482]
	{0x0F12, 0x004A},	//#TVAR_ash_pGAS[483]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[484]
	{0x0F12, 0x0018},	//#TVAR_ash_pGAS[485]
	{0x0F12, 0x000A},	//#TVAR_ash_pGAS[486]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[487]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[488]
	{0x0F12, 0x001F},	//#TVAR_ash_pGAS[489]
	{0x0F12, 0x0039},	//#TVAR_ash_pGAS[490]
	{0x0F12, 0x005B},	//#TVAR_ash_pGAS[491]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[492]
	{0x0F12, 0x00B6},	//#TVAR_ash_pGAS[493]
	{0x0F12, 0x009C},	//#TVAR_ash_pGAS[494]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[495]
	{0x0F12, 0x0045},	//#TVAR_ash_pGAS[496]
	{0x0F12, 0x0028},	//#TVAR_ash_pGAS[497]
	{0x0F12, 0x0012},	//#TVAR_ash_pGAS[498]
	{0x0F12, 0x0004},	//#TVAR_ash_pGAS[499]
	{0x0F12, 0x0000},	//#TVAR_ash_pGAS[500]
	{0x0F12, 0x0006},	//#TVAR_ash_pGAS[501]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[502]
	{0x0F12, 0x0030},	//#TVAR_ash_pGAS[503]
	{0x0F12, 0x0050},	//#TVAR_ash_pGAS[504]
	{0x0F12, 0x007B},	//#TVAR_ash_pGAS[505]
	{0x0F12, 0x00AB},	//#TVAR_ash_pGAS[506]
	{0x0F12, 0x009F},	//#TVAR_ash_pGAS[507]
	{0x0F12, 0x0071},	//#TVAR_ash_pGAS[508]
	{0x0F12, 0x0048},	//#TVAR_ash_pGAS[509]
	{0x0F12, 0x002B},	//#TVAR_ash_pGAS[510]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[511]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[512]
	{0x0F12, 0x0001},	//#TVAR_ash_pGAS[513]
	{0x0F12, 0x0007},	//#TVAR_ash_pGAS[514]
	{0x0F12, 0x0016},	//#TVAR_ash_pGAS[515]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[516]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[517]
	{0x0F12, 0x0079},	//#TVAR_ash_pGAS[518]
	{0x0F12, 0x00A8},	//#TVAR_ash_pGAS[519]
	{0x0F12, 0x00A9},	//#TVAR_ash_pGAS[520]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[521]
	{0x0F12, 0x0054},	//#TVAR_ash_pGAS[522]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[523]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[524]
	{0x0F12, 0x0013},	//#TVAR_ash_pGAS[525]
	{0x0F12, 0x000E},	//#TVAR_ash_pGAS[526]
	{0x0F12, 0x0015},	//#TVAR_ash_pGAS[527]
	{0x0F12, 0x0022},	//#TVAR_ash_pGAS[528]
	{0x0F12, 0x0038},	//#TVAR_ash_pGAS[529]
	{0x0F12, 0x0055},	//#TVAR_ash_pGAS[530]
	{0x0F12, 0x0080},	//#TVAR_ash_pGAS[531]
	{0x0F12, 0x00AE},	//#TVAR_ash_pGAS[532]
	{0x0F12, 0x00BE},	//#TVAR_ash_pGAS[533]
	{0x0F12, 0x0097},	//#TVAR_ash_pGAS[534]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[535]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[536]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[537]
	{0x0F12, 0x002D},	//#TVAR_ash_pGAS[538]
	{0x0F12, 0x0029},	//#TVAR_ash_pGAS[539]
	{0x0F12, 0x002E},	//#TVAR_ash_pGAS[540]
	{0x0F12, 0x003B},	//#TVAR_ash_pGAS[541]
	{0x0F12, 0x004D},	//#TVAR_ash_pGAS[542]
	{0x0F12, 0x006A},	//#TVAR_ash_pGAS[543]
	{0x0F12, 0x0092},	//#TVAR_ash_pGAS[544]
	{0x0F12, 0x00BB},	//#TVAR_ash_pGAS[545]
	{0x0F12, 0x00D3},	//#TVAR_ash_pGAS[546]
	{0x0F12, 0x00B2},	//#TVAR_ash_pGAS[547]
	{0x0F12, 0x008A},	//#TVAR_ash_pGAS[548]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[549]
	{0x0F12, 0x0058},	//#TVAR_ash_pGAS[550]
	{0x0F12, 0x004C},	//#TVAR_ash_pGAS[551]
	{0x0F12, 0x0049},	//#TVAR_ash_pGAS[552]
	{0x0F12, 0x004E},	//#TVAR_ash_pGAS[553]
	{0x0F12, 0x005A},	//#TVAR_ash_pGAS[554]
	{0x0F12, 0x006D},	//#TVAR_ash_pGAS[555]
	{0x0F12, 0x0089},	//#TVAR_ash_pGAS[556]
	{0x0F12, 0x00AC},	//#TVAR_ash_pGAS[557]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[558]
	{0x0F12, 0x00F5},	//#TVAR_ash_pGAS[559]
	{0x0F12, 0x00D4},	//#TVAR_ash_pGAS[560]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[561]
	{0x0F12, 0x008E},	//#TVAR_ash_pGAS[562]
	{0x0F12, 0x007C},	//#TVAR_ash_pGAS[563]
	{0x0F12, 0x0070},	//#TVAR_ash_pGAS[564]
	{0x0F12, 0x006B},	//#TVAR_ash_pGAS[565]
	{0x0F12, 0x006F},	//#TVAR_ash_pGAS[566]
	{0x0F12, 0x007E},	//#TVAR_ash_pGAS[567]
	{0x0F12, 0x0093},	//#TVAR_ash_pGAS[568]
	{0x0F12, 0x00AD},	//#TVAR_ash_pGAS[569]
	{0x0F12, 0x00CD},	//#TVAR_ash_pGAS[570]
	{0x0F12, 0x00E4},	//#TVAR_ash_pGAS[571]



	//=================================================================================================
	//	Set Gamma LUT
	//=================================================================================================
	//						//New	//Hong	//ORG
	{0x002A, 0x04CC},
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[0][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[0][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[0][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[0][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[0][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[0][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[0][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[0][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[0][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[0][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[0][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[0][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[0][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[0][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[0][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[0][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[1][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[1][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[1][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[1][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[1][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[1][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[1][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[1][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[1][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[1][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[1][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[1][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[1][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[1][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[1][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[1][15]
	{0x0F12, 0x0000},	//#SARR_usGammaLutRGBIndoor[2][0]
	{0x0F12, 0x000B},	//#SARR_usGammaLutRGBIndoor[2][1]
	{0x0F12, 0x0013},	//#SARR_usGammaLutRGBIndoor[2][2]
	{0x0F12, 0x0023},	//#SARR_usGammaLutRGBIndoor[2][3]
	{0x0F12, 0x004D},	//#SARR_usGammaLutRGBIndoor[2][4]
	{0x0F12, 0x00AC},	//#SARR_usGammaLutRGBIndoor[2][5]
	{0x0F12, 0x0108},	//#SARR_usGammaLutRGBIndoor[2][6]
	{0x0F12, 0x017F},	//#SARR_usGammaLutRGBIndoor[2][7]
	{0x0F12, 0x020C},	//#SARR_usGammaLutRGBIndoor[2][8]
	{0x0F12, 0x026F},	//#SARR_usGammaLutRGBIndoor[2][9]
	{0x0F12, 0x02F2},	//#SARR_usGammaLutRGBIndoor[2][10]
	{0x0F12, 0x0339},	//#SARR_usGammaLutRGBIndoor[2][11]
	{0x0F12, 0x037B},	//#SARR_usGammaLutRGBIndoor[2][12]
	{0x0F12, 0x03AE},	//#SARR_usGammaLutRGBIndoor[2][13]
	{0x0F12, 0x03DD},	//#SARR_usGammaLutRGBIndoor[2][14]
	{0x0F12, 0x03FE},	//#SARR_usGammaLutRGBIndoor[2][15]



	//=================================================================================================
	//	Set AWB
	//=================================================================================================
	{0x002A, 0x0DA8},
	{0x0F12, 0x0020},	// #awbb_LowBr0_NBzone	// 2008.04.24. eliminate to the purple color at low light.
	{0x002A, 0x0E8C},
	{0x0F12, 0x0000},	// #awbb_LowBr0_PatchNumZone
	{0x002A, 0x0D6C},
	{0x0F12, 0x0040},	// #awbb_YMedMoveToYAv

	// Indoor Gray Zone
	{0x002A, 0x0B9C},
	{0x0F12, 0x0339},	//0392	// #awbb_IndoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x0393},	//03A4	// #awbb_IndoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x031B},	//036F	// #awbb_IndoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x039C},	//03AE	// #awbb_IndoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x030A},	//034E	// #awbb_IndoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0398},	//03A2	// #awbb_IndoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x02F3},	//0331	// #awbb_IndoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0392},	//0392	// #awbb_IndoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x02E0},	//0315	// #awbb_IndoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0388},	//0381	// #awbb_IndoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x02CB},	//02F8	// #awbb_IndoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0374},	//036C	// #awbb_IndoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x02BB},	//02E1	// #awbb_IndoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x034B},	//0358	// #awbb_IndoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x02AA},	//02C5	// #awbb_IndoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x0336},	//033B	// #awbb_IndoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x029C},	//02B1	// #awbb_IndoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0321},	//0323	// #awbb_IndoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x028B},	//029C	// #awbb_IndoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x030B},	//030F	// #awbb_IndoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x027B},	//028B	// #awbb_IndoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02FD},	//02FA	// #awbb_IndoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x0268},	//027C	// #awbb_IndoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x02EA},	//02E2	// #awbb_IndoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x0259},	//0268	// #awbb_IndoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x02D8},	//02D1	// #awbb_IndoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x024C},	//0259	// #awbb_IndoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x02CB},	//02BF	// #awbb_IndoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x023D},	//024C	// #awbb_IndoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x02BD},	//02B1	// #awbb_IndoorGrZones_m_BGrid_14__m_right
	{0x0F12, 0x0231},	//023D	// #awbb_IndoorGrZones_m_BGrid_15__m_left
	{0x0F12, 0x02B1},	//02A2	// #awbb_IndoorGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0222},	//0231	// #awbb_IndoorGrZones_m_BGrid_16__m_left
	{0x0F12, 0x02A8},	//0299	// #awbb_IndoorGrZones_m_BGrid_16__m_right
	{0x0F12, 0x0215},	//0222	// #awbb_IndoorGrZones_m_BGrid_17__m_left
	{0x0F12, 0x029B},	//0290	// #awbb_IndoorGrZones_m_BGrid_17__m_right
	{0x0F12, 0x0208},	//0215	// #awbb_IndoorGrZones_m_BGrid_18__m_left
	{0x0F12, 0x0290},	//0288	// #awbb_IndoorGrZones_m_BGrid_18__m_right
	{0x0F12, 0x01F7},	//0208	// #awbb_IndoorGrZones_m_BGrid_19__m_left
	{0x0F12, 0x0287},	//0281	// #awbb_IndoorGrZones_m_BGrid_19__m_right
	{0x0F12, 0x01E6},	//01F7	// #awbb_IndoorGrZones_m_BGrid_20__m_left
	{0x0F12, 0x027F},	//0279	// #awbb_IndoorGrZones_m_BGrid_20__m_right
	{0x0F12, 0x01D9},	//01E6	// #awbb_IndoorGrZones_m_BGrid_21__m_left
	{0x0F12, 0x0279},	//026E	// #awbb_IndoorGrZones_m_BGrid_21__m_right
	{0x0F12, 0x01CC},	//01D9	// #awbb_IndoorGrZones_m_BGrid_22__m_left
	{0x0F12, 0x0272},	//0268	// #awbb_IndoorGrZones_m_BGrid_22__m_right
	{0x0F12, 0x01C1},	//01CC	// #awbb_IndoorGrZones_m_BGrid_23__m_left
	{0x0F12, 0x026C},	//0263	// #awbb_IndoorGrZones_m_BGrid_23__m_right
	{0x0F12, 0x01B3},	//01C1	// #awbb_IndoorGrZones_m_BGrid_24__m_left
	{0x0F12, 0x0265},	//0260	// #awbb_IndoorGrZones_m_BGrid_24__m_right
	{0x0F12, 0x01AA},	//01B3	// #awbb_IndoorGrZones_m_BGrid_25__m_left
	{0x0F12, 0x025C},	//0259	// #awbb_IndoorGrZones_m_BGrid_25__m_right
	{0x0F12, 0x019F},	//01AA	// #awbb_IndoorGrZones_m_BGrid_26__m_left
	{0x0F12, 0x0250},	//0253	// #awbb_IndoorGrZones_m_BGrid_26__m_right
	{0x0F12, 0x019B},	//019F	// #awbb_IndoorGrZones_m_BGrid_27__m_left
	{0x0F12, 0x0247},	//0248	// #awbb_IndoorGrZones_m_BGrid_27__m_right
	{0x0F12, 0x0197},	//019B	// #awbb_IndoorGrZones_m_BGrid_28__m_left
	{0x0F12, 0x0239},	//023F	// #awbb_IndoorGrZones_m_BGrid_28__m_right
	{0x0F12, 0x019B},	//0197	// #awbb_IndoorGrZones_m_BGrid_29__m_left
	{0x0F12, 0x021D},	//0239	// #awbb_IndoorGrZones_m_BGrid_29__m_right
	{0x0F12, 0x019D},	//019B	// #awbb_IndoorGrZones_m_BGrid_30__m_left
	{0x0F12, 0x0202},	//021D	// #awbb_IndoorGrZones_m_BGrid_30__m_right
	{0x0F12, 0x01AC},	//019D	// #awbb_IndoorGrZones_m_BGrid_31__m_left
	{0x0F12, 0x01D3},	//020A	// #awbb_IndoorGrZones_m_BGrid_31__m_right
	{0x0F12, 0x0000},	//01AC	// #awbb_IndoorGrZones_m_BGrid_32__m_left
	{0x0F12, 0x0000},	//01D3	// #awbb_IndoorGrZones_m_BGrid_32__m_right
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_left
	{0x0F12, 0x0000},	//0000	// #awbb_IndoorGrZones_m_BGrid_33__m_right

	{0x002A, 0x0C3C},
	{0x0F12, 0x0004},	// #awbb_IndoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0138},	// #awbb_IndoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x0F12, 0x0000},

	// Outdoor Gray Zone
	{0x0F12, 0x0255},	//#awbb_OutdoorGrZones_m_BGrid_0__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_0__m_right
	{0x0F12, 0x0236},	//#awbb_OutdoorGrZones_m_BGrid_1__m_left
	{0x0F12, 0x0290},	//#awbb_OutdoorGrZones_m_BGrid_1__m_right
	{0x0F12, 0x0227},	//#awbb_OutdoorGrZones_m_BGrid_2__m_left
	{0x0F12, 0x0297},	//#awbb_OutdoorGrZones_m_BGrid_2__m_right
	{0x0F12, 0x0210},	//#awbb_OutdoorGrZones_m_BGrid_3__m_left
	{0x0F12, 0x0293},	//#awbb_OutdoorGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0200},	//#awbb_OutdoorGrZones_m_BGrid_4__m_left
	{0x0F12, 0x028B},	//#awbb_OutdoorGrZones_m_BGrid_4__m_right
	{0x0F12, 0x01F1},	//#awbb_OutdoorGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0286},	//#awbb_OutdoorGrZones_m_BGrid_5__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_6__m_left
	{0x0F12, 0x0282},	//#awbb_OutdoorGrZones_m_BGrid_6__m_right
	{0x0F12, 0x01E0},	//#awbb_OutdoorGrZones_m_BGrid_7__m_left
	{0x0F12, 0x027B},	//#awbb_OutdoorGrZones_m_BGrid_7__m_right
	{0x0F12, 0x01DE},	//#awbb_OutdoorGrZones_m_BGrid_8__m_left
	{0x0F12, 0x0274},	//#awbb_OutdoorGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01DF},	//#awbb_OutdoorGrZones_m_BGrid_9__m_left
	{0x0F12, 0x026F},	//#awbb_OutdoorGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01E7},	//#awbb_OutdoorGrZones_m_BGrid_10__m_left
	{0x0F12, 0x0263},	//#awbb_OutdoorGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01F6},	//#awbb_OutdoorGrZones_m_BGrid_11__m_left
	{0x0F12, 0x0253},	//#awbb_OutdoorGrZones_m_BGrid_11__m_right
	{0x0F12, 0x020B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_left
	{0x0F12, 0x023B},	//#awbb_OutdoorGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0000},	//#awbb_OutdoorGrZones_m_BGrid_14__m_right

	{0x002A, 0x0CB8},
	{0x0F12, 0x0004},	// #awbb_OutdoorGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x000C},	//#awbb_OutdoorGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0217},	//#awbb_OutdoorGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high
	{0x0F12, 0x0000},

	// Low Brightness Gray Zone
	{0x0F12, 0x0324},	//031E  // #awbb_LowBrGrZones_m_BGrid_0__m_left
	{0x0F12, 0x03A1},	//037E  // #awbb_LowBrGrZones_m_BGrid_0__m_right
	{0x0F12, 0x02F2},	//02CF  // #awbb_LowBrGrZones_m_BGrid_1__m_left
	{0x0F12, 0x03BA},	//0389  // #awbb_LowBrGrZones_m_BGrid_1__m_right
	{0x0F12, 0x02C4},	//02A6  // #awbb_LowBrGrZones_m_BGrid_2__m_left
	{0x0F12, 0x03B6},	//0386  // #awbb_LowBrGrZones_m_BGrid_2__m_right
	{0x0F12, 0x029E},	//0282  // #awbb_LowBrGrZones_m_BGrid_3__m_left
	{0x0F12, 0x03A6},	//0374  // #awbb_LowBrGrZones_m_BGrid_3__m_right
	{0x0F12, 0x0283},	//0259  // #awbb_LowBrGrZones_m_BGrid_4__m_left
	{0x0F12, 0x0380},	//0352  // #awbb_LowBrGrZones_m_BGrid_4__m_right
	{0x0F12, 0x025F},	//0239  // #awbb_LowBrGrZones_m_BGrid_5__m_left
	{0x0F12, 0x0344},	//0330  // #awbb_LowBrGrZones_m_BGrid_5__m_right
	{0x0F12, 0x0241},	//021E  // #awbb_LowBrGrZones_m_BGrid_6__m_left
	{0x0F12, 0x031F},	//031B  // #awbb_LowBrGrZones_m_BGrid_6__m_right
	{0x0F12, 0x0227},	//020B  // #awbb_LowBrGrZones_m_BGrid_7__m_left
	{0x0F12, 0x02FC},	//0300  // #awbb_LowBrGrZones_m_BGrid_7__m_right
	{0x0F12, 0x020A},	//01F1  // #awbb_LowBrGrZones_m_BGrid_8__m_left
	{0x0F12, 0x02D9},	//02DD  // #awbb_LowBrGrZones_m_BGrid_8__m_right
	{0x0F12, 0x01F1},	//01E0  // #awbb_LowBrGrZones_m_BGrid_9__m_left
	{0x0F12, 0x02C4},	//02C4  // #awbb_LowBrGrZones_m_BGrid_9__m_right
	{0x0F12, 0x01D0},	//01CD  // #awbb_LowBrGrZones_m_BGrid_10__m_left
	{0x0F12, 0x02B2},	//02AF  // #awbb_LowBrGrZones_m_BGrid_10__m_right
	{0x0F12, 0x01B8},	//019E  // #awbb_LowBrGrZones_m_BGrid_11__m_left
	{0x0F12, 0x029E},	//02A0  // #awbb_LowBrGrZones_m_BGrid_11__m_right
	{0x0F12, 0x019F},	//0187  // #awbb_LowBrGrZones_m_BGrid_12__m_left
	{0x0F12, 0x0290},	//028C  // #awbb_LowBrGrZones_m_BGrid_12__m_right
	{0x0F12, 0x0189},	//017D  // #awbb_LowBrGrZones_m_BGrid_13__m_left
	{0x0F12, 0x0282},	//027E  // #awbb_LowBrGrZones_m_BGrid_13__m_right
	{0x0F12, 0x0182},	//0179  // #awbb_LowBrGrZones_m_BGrid_14__m_left
	{0x0F12, 0x0271},	//0258  // #awbb_LowBrGrZones_m_BGrid_14__m_right
	{0x0F12, 0x017F},	//0186  // #awbb_LowBrGrZones_m_BGrid_15__m_left
	{0x0F12, 0x0254},	//0232  // #awbb_LowBrGrZones_m_BGrid_15__m_right
	{0x0F12, 0x0183},	//01BD  // #awbb_LowBrGrZones_m_BGrid_16__m_left
	{0x0F12, 0x022D},	//01FB  // #awbb_LowBrGrZones_m_BGrid_16__m_right
	{0x0F12, 0x019C},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_left
	{0x0F12, 0x01D3},	//0000  // #awbb_LowBrGrZones_m_BGrid_17__m_right

	{0x002A, 0x0D34},
	{0x0F12, 0x0005},	// #awbb_LowBrGrZones_m_GridStep
	{0x0F12, 0x0000},
	{0x0F12, 0x0012},	// #awbb_LowBrGrZones_m_GridSz
	{0x0F12, 0x0000},
	{0x0F12, 0x0120},	// #awbb_LowBrGrZones_m_Boffs
	{0x0F12, 0x0000},
	{0x0F12, 0x0002},	// #awbb_LowBrGrZones_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D48},
	{0x0F12, 0x00E0},	// #awbb_LowBrGrZones_y_high
	{0x0F12, 0x0000},

	// Lowtemp Circle
	{0x0F12, 0x032F},	// #awbb_CrclLowT_R_c
	{0x0F12, 0x0000},
	{0x0F12, 0x017A},	// #awbb_CrclLowT_B_c
	{0x0F12, 0x0000},
	{0x0F12, 0x7300},	// #awbb_CrclLowT_Rad_c
	{0x0F12, 0x0000},
	{0x0F12, 0x000A},	// #awbb_CrclLowT_y_low
	{0x0F12, 0x0000},
	{0x002A, 0x0D60},
	{0x0F12, 0x00E0},	// #awbb_CrclLowT_y_high
	{0x0F12, 0x0000},
	{0x002A, 0x0D82},
	{0x0F12, 0x0001},	// #awbb_ByPass_LowTempMode

	// White Locus
	{0x002A, 0x0D66},
	{0x0F12, 0x011F},	// #awbb_IntcR
	{0x0F12, 0x0132},	// #awbb_IntcB
	{0x002A, 0x0D74},
	{0x0F12, 0x052A},	// #awbb_MvEq_RBthresh

	// Gamut Thresholds
	{0x002A, 0x0DAE},
	{0x0F12, 0x0036},	// #awbb_GamutWidthThr2
	{0x0F12, 0x001C},	// #awbb_GamutHeightThr2
	{0x002A, 0x0DAA},
	{0x0F12, 0x071A},	// #awbb_GamutWidthThr1
	{0x0F12, 0x03A4},	// #awbb_GamutHeightThr1

	// SceneDetection Thresholds
	{0x002A, 0x0D92},
	{0x0F12, 0x0CE4},	//0A8C	// #awbb_SunnyBr
	{0x0F12, 0x0096},	// #awbb_Sunny_NBzone
	{0x002A, 0x0E86},
	{0x0F12, 0x0216},	// #awbb_OutdoorWP_r
	{0x0F12, 0x029F},	// #awbb_OutdoorWP_b
	{0x002A, 0x0D96},
	{0x0F12, 0x0580},	// #awbb_CloudyBr
	{0x0F12, 0x0096},	// #awbb_Cloudy_NBzone
	{0x002A, 0x0DB2},
	{0x0F12, 0x00DA},	// #awbb_CloudyRB
	{0x002A, 0x0D9A},
	{0x0F12, 0x000A},	// #awbb_Cloudy_BdivRzone
	{0x002A, 0x0DB4},
	{0x0F12, 0x0459},	// #awbb_LowTempRB
	{0x002A, 0x0DA4},
	{0x0F12, 0x000E},	// #awbb_LowTemp_RBzone
	{0x002A, 0x0D64},
	{0x0F12, 0x0040},	// #awbb_DarkBr
	{0x002A, 0x0DA6},
	{0x0F12, 0x001E},	// #awbb_LowBr_NBzone
	{0x002A, 0x0D9C},
	{0x0F12, 0x001B},	// #awbb_MacbethGamut_WidthZone
	{0x0F12, 0x000E},	// #awbb_MacbethGamut_HeightZone
	{0x0F12, 0x0008},	// #awbb_MacbethGamut_WidthZone2
	{0x0F12, 0x0004},	// #awbb_MacbethGamut_HeightZone2

	// AWB Debug.(Outdoor Pink)
	{0x002A, 0x0E30},
	{0x0F12, 0x0000},	//	#awbb_OutdoorFltrSz (outdoor WB를 moving average filtering)

	// UseInvalidOutdoor option
	{0x002A, 0x0D88},
	{0x0F12, 0x0001},	// #awbb_Use_InvalidOutDoor

	// AWB input Y-Filter setting
	{0x002A, 0x0C48},
	{0x0F12, 0x0020},	// #awbb_IndoorGrZones_y_low
	{0x002A, 0x0C50},
	{0x0F12, 0x00E0},	// #awbb_IndoorGrZones_y_high
	{0x002A, 0x0CC4},
	{0x0F12, 0x0020},	// #awbb_OutdoorGrZones_y_low
	{0x002A, 0x0CCC},
	{0x0F12, 0x00C0},	// #awbb_OutdoorGrZones_y_high

	{0x002A, 0x0DC2},
	{0x0F12, 0x0030},	// #awbb_GnCurPntImmunity
	{0x0F12, 0x00C8},	// #awbb_GnFarPntImmunity
	{0x0F12, 0x012C},	// #awbb_GnCurPntLongJump
	{0x0F12, 0x0258},	// #awbb_GainsMaxMove
	{0x0F12, 0x0003},	// #awbb_GnMinMatchToJump

	// RB Gain Offset
	//WRITE 70000E32	FFE0	//#awbb_RGainOff
	//WRITE 70000E34	FFe0	//#awbb_BGainOff


	//=================================================================================================
	//	Set CCM
	//=================================================================================================
	// CCM Start Address
	{0x002A, 0x06D0},
	{0x0F12, 0x2800},   //#TVAR_wbt_pBaseCcmsAddr[0]
	{0x0F12, 0x7000},
	{0x0F12, 0x2824},   //#TVAR_wbt_pBaseCcmsAddr[1]
	{0x0F12, 0x7000},
	{0x0F12, 0x2848},   //#TVAR_wbt_pBaseCcmsAddr[2]
	{0x0F12, 0x7000},
	{0x0F12, 0x286C},   //#TVAR_wbt_pBaseCcmsAddr[3]
	{0x0F12, 0x7000},
	{0x0F12, 0x2890},   //#TVAR_wbt_pBaseCcmsAddr[4]
	{0x0F12, 0x7000},
	{0x0F12, 0x28B4},   //#TVAR_wbt_pBaseCcmsAddr[5]
	{0x0F12, 0x7000},
	{0x002A, 0x06EC},
	{0x0F12, 0x28D8},   //#TVAR_wbt_pOutdoorCcm
	{0x0F12, 0x7000},

	{0x002A, 0x2800},
	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01F6},
	{0x0F12, 0xFF76},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00BD},
	{0x0F12, 0x00E8},
	{0x0F12, 0xFF26},
	{0x0F12, 0xFF28},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFC2},
	{0x0F12, 0xFE22},
	{0x0F12, 0x0132},
	{0x0F12, 0x0132},
	{0x0F12, 0x0017},
	{0x0F12, 0xFFA9},
	{0x0F12, 0x01FA},
	{0x0F12, 0x018F},
	{0x0F12, 0xFF03},
	{0x0F12, 0x0117},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01FC},
	{0x0F12, 0xFF75},
	{0x0F12, 0xFFED},
	{0x0F12, 0x00C3},
	{0x0F12, 0x00F1},
	{0x0F12, 0xFF10},
	{0x0F12, 0xFEFD},
	{0x0F12, 0x0158},
	{0x0F12, 0xFFB2},
	{0x0F12, 0xFDFD},
	{0x0F12, 0x0142},
	{0x0F12, 0x0142},
	{0x0F12, 0x0011},
	{0x0F12, 0xFF9C},
	{0x0F12, 0x0214},
	{0x0F12, 0x01A5},
	{0x0F12, 0xFEEC},
	{0x0F12, 0x0125},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEEE},
	{0x0F12, 0x01AA},
	{0x0F12, 0xFFAA},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},

	// Outdoor CCM
	{0x0F12, 0x01B3},
	{0x0F12, 0xFF9E},
	{0x0F12, 0xFFF7},
	{0x0F12, 0x00CB},
	{0x0F12, 0x00ED},
	{0x0F12, 0xFF54},
	{0x0F12, 0xFEA4},
	{0x0F12, 0x020F},
	{0x0F12, 0xFF8F},
	{0x0F12, 0xFE86},
	{0x0F12, 0x0128},
	{0x0F12, 0x0128},
	{0x0F12, 0xFFE5},
	{0x0F12, 0xFFE5},
	{0x0F12, 0x01CA},
	{0x0F12, 0x0173},
	{0x0F12, 0xFF37},
	{0x0F12, 0x0114},



	//=================================================================================================
	//	Set NB
	//=================================================================================================
	{0x002A, 0x07EA},
	{0x0F12, 0x0000},	//afit_bUseNoiseInd 0 : NB 1: Noise Index

	// param_start	SARR_uNormBrInDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrInDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrInDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrInDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrInDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrInDoor[4]

	// param_start	SARR_uNormBrOutDoor
	{0x0F12, 0x000A},	// #SARR_uNormBrOutDoor[0]
	{0x0F12, 0x0019},	// #SARR_uNormBrOutDoor[1]
	{0x0F12, 0x007D},	// #SARR_uNormBrOutDoor[2]
	{0x0F12, 0x01F4},	// #SARR_uNormBrOutDoor[3]
	{0x0F12, 0x1388},	// #SARR_uNormBrOutDoor[4]



	//=================================================================================================
	//	Set AFIT
	//=================================================================================================
	// AFIT Start Address
	{0x002A, 0x0814},
	{0x0F12, 0x082C},	//#TVAR_afit_pBaseVals
	{0x0F12, 0x7000},	//#TVAR_afit_pBaseVals

	// AFIT Table	//0223
	{0x002A, 0x082C},
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0000},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0028},	//SHARP_BLUR
	{0x0F12, 0xFFE2},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0028},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                   bin: desparity low
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"                bin: desparity high
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x06FF},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x030A},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"               ]negati fine
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"                  ]low fine
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"                ]high fine
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"                   ]high low thres
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0005},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01AA},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0000},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x001E},	//SHARP_BLUR
	{0x0F12, 0xFFE2},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0014},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x000E},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0096},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0344},	//UVDenoise_iYLowThresh
	{0x0F12, 0x033A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"                  bin: desparity low
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0303},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0303},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x0A05},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x0F05},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x08B7},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0308},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x01A0},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8035},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x000E},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0089},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0202},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0202},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0200},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0206},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x2000},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0600},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D08},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x803C},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0006},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0014},	//UVDenoise_iYLowThresh
	{0x0F12, 0x000A},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0205},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0000},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x8050},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x0000},	//BRIGHTNESS
	{0x0F12, 0x0008},	//CONTRAST
	{0x0F12, 0x0000},	//SATURATION
	{0x0F12, 0x0006},	//SHARP_BLUR
	{0x0F12, 0xFFF1},	//GLAMOUR
	{0x0F12, 0x03FF},	//Disparity_iSatSat
	{0x0F12, 0x0000},	//Denoise1_iYDenThreshLow
	{0x0F12, 0x0006},	//Denoise1_iYDenThreshLow_Bin
	{0x0F12, 0x0077},	//Denoise1_iYDenThreshHigh
	{0x0F12, 0x0050},	//Denoise1_iYDenThreshHigh_Bin
	{0x0F12, 0x0002},	//Denoise1_iLowWWideThresh
	{0x0F12, 0x0002},	//Denoise1_iHighWWideThresh
	{0x0F12, 0x000A},	//Denoise1_iLowWideThresh
	{0x0F12, 0x000A},	//Denoise1_iHighWideThresh
	{0x0F12, 0x03FF},	//Denoise1_iSatSat
	{0x0F12, 0x03FF},	//Demosaic4_iHystGrayLow
	{0x0F12, 0x0000},	//Demosaic4_iHystGrayHigh
	{0x0F12, 0x0000},	//UVDenoise_iYLowThresh
	{0x0F12, 0x0000},	//UVDenoise_iYHighThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVLowThresh
	{0x0F12, 0x03FF},	//UVDenoise_iUVHighThresh
	{0x0F12, 0x0028},	//DSMix1_iLowLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Wide_Bin
	{0x0F12, 0x0014},	//DSMix1_iHighLimit_Wide
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Wide_Bin
	{0x0F12, 0x0050},	//DSMix1_iLowLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iLowLimit_Fine_Bin
	{0x0F12, 0x0046},	//DSMix1_iHighLimit_Fine
	{0x0F12, 0x0032},	//DSMix1_iHighLimit_Fine_Bin
	{0x0F12, 0x0106},	//DSMix1_iRGBOffset
	{0x0F12, 0x006F},	//DSMix1_iDemClamp
	{0x0F12, 0x0202},	//"Disparity_iDispTH_LowDisparity_iDispTH_Low_Bin"
	{0x0F12, 0x0502},	//"Disparity_iDispTH_High Disparity_iDispTH_High_Bin"
	{0x0F12, 0x0102},	//"Despeckle_iCorrectionLevelColdDespeckle_iCorrectionLevelCold_Bin"
	{0x0F12, 0x0102},	//Despeckle_iCorrectionLevelHotDespeckle_iCorrectionLevelHot_Bin
	{0x0F12, 0x140A},	//"Despeckle_iColdThreshLowDespeckle_iColdThreshHigh"
	{0x0F12, 0x140A},	//"Despeckle_iHotThreshLowDespeckle_iHotThreshHigh"
	{0x0F12, 0x2828},	//"Denoise1_iLowMaxSlopeAllowedDenoise1_iHighMaxSlopeAllowed"
	{0x0F12, 0x1A1E},	//"Denoise1_iLowSlopeThreshDenoise1_iHighSlopeThresh"
	{0x0F12, 0x0203},	//"Denoise1_iRadialPowerDenoise1_iRadialDivideShift"
	{0x0F12, 0x0880},	//"Denoise1_iRadialLimitDenoise1_iLWBNoise"
	{0x0F12, 0x0003},	//"Denoise1_iWideDenoise1_iWideWide"
	{0x0F12, 0x0105},	//"Demosaic4_iHystGrayRangeUVDenoise_iYSupport"
	{0x0F12, 0x2803},	//"UVDenoise_iUVSupportDSMix1_iLowPower_Wide"
	{0x0F12, 0x2811},	//"DSMix1_iLowPower_Wide_BinDSMix1_iHighPower_Wide"
	{0x0F12, 0x0A0F},	//"DSMix1_iHighPower_Wide_BinDSMix1_iLowThresh_Wide"
	{0x0F12, 0x050A},	//"DSMix1_iHighThresh_WideDSMix1_iReduceNegativeWide"
	{0x0F12, 0x200F},	//"DSMix1_iLowPower_FineDSMix1_iLowPower_Fine_Bin"
	{0x0F12, 0x2032},	//"DSMix1_iHighPower_FineDSMix1_iHighPower_Fine_Bin"
	{0x0F12, 0x0A0A},	//"DSMix1_iLowThresh_FineDSMix1_iHighThresh_Fine"
	{0x0F12, 0x0005},	//"DSMix1_iReduceNegativeFineDSMix1_iRGBMultiplier"
	{0x0F12, 0x0405},	//"Mixer1_iNLowNoisePowerMixer1_iNLowNoisePower_Bin"
	{0x0F12, 0x0406},	//"Mixer1_iNVeryLowNoisePowerMixer1_iNVeryLowNoisePower_Bin"
	{0x0F12, 0x0001},	//"Mixer1_iNHighNoisePowerMixer1_iNHighNoisePower_Bin"
	{0x0F12, 0x0808},	//"Mixer1_iWLowNoisePowerMixer1_iWVeryLowNoisePower"
	{0x0F12, 0x0200},	//"Mixer1_iWHighNoisePowerMixer1_iWLowNoiseCeilGain"
	{0x0F12, 0x0D06},	//"Mixer1_iWHighNoiseCeilGainMixer1_iWNoiseCeilGain"
	{0x0F12, 0x0164},	//"CCM_Oscar_iSaturationCCM_Oscar_bSaturation"
	{0x0F12, 0x0180},	//"RGBGamma2_iLinearityRGBGamma2_bLinearity"
	{0x0F12, 0x0100},	//"RGBGamma2_iDarkReduceRGBGamma2_bDarkReduce"
	{0x0F12, 0x805A},	//"byr_gas2_iShadingPowerRGB2YUV_iRGBGain"
	{0x0F12, 0x0180},	//"RGB2YUV_iSaturationRGB2YUV_bGainOffset"
	{0x0F12, 0x0000},	//RGB2YUV_iYOffset
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[0]
	{0x0F12, 0x00FF},	// # afit_pConstBaseVals[1]
	{0x0F12, 0x0800},	// # afit_pConstBaseVals[2]
	{0x0F12, 0x0600},	// # afit_pConstBaseVals[3]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[4]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[5]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[6]
	{0x0F12, 0x0300},	// # afit_pConstBaseVals[7]
	{0x0F12, 0x0002},	// # afit_pConstBaseVals[8]
	{0x0F12, 0x0400},	// # afit_pConstBaseVals[9]
	{0x0F12, 0x0106},	// # afit_pConstBaseVals[10]
	{0x0F12, 0x0005},	// # afit_pConstBaseVals[11]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[12]
	{0x0F12, 0x0003},	// # afit_pConstBaseVals[13]
	{0x0F12, 0x0000},	// # afit_pConstBaseVals[14]
	{0x0F12, 0xFFD6},	// # afit_pConstBaseVals[15]
	{0x0F12, 0x53C1},	// # afit_pConstBaseVals[16]
	{0x0F12, 0xE1FE},	// # afit_pConstBaseVals[17]
	{0x0F12, 0x0001},	// # afit_pConstBaseVals[18]

	{0x002A, 0x01EA},
	{0x0F12, 0x0000},	//#REG_TC_UserSharpBlur
	{0x002A, 0x0E32},
	{0x0F12, 0xFFF0},	//#awbb_RGainOff
	{0x0F12, 0x0000},	//#awbb_BGainOff

	// For fast AE
	{0x002A, 0x0FAC},
	{0x0F12, 0x0001},	// AE Gain output [0]    default : 0x10
	{0x0F12, 0x0008},	// AE Gain output [1]    default : 0x20
	{0x0F12, 0x0015},	// AE Gain output [2]    default : 0x40

	// For fast AWB
	{0x002A, 0x0DCA},
	{0x0F12, 0x0001},	// awbb_GnMinMatchToJump = 0x02 (default:0x03)
	{0x0028, 0x7000},
	{0x002A, 0x0E98},
	{0x0F12, 0x0300},	//bp_uMaxBrightnessFactor 0180 -> 0300
	{0x002A, 0x0E9E},
	{0x0F12, 0x0300},	//bp_uMinBrightnessFactor 0180 -> 0300

	// Update Changed Registers
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},	//#REG_TC_DBG_ReInitCmd
	{0x002A, 0x03FC},
	{0x0F12, 0x0001},


	{0x0028, 0xD000},
	{0x002A, 0x1088},	// PCLK_CD
	{0x0F12, 0x05D5},

	{0x0028, 0xD000},
	{0x002A, 0x1094},	//PCLK_Delay
	{0x0F12, 0x0030},

	{0x0028, 0xD000},
	{0x002A, 0x1082},	// Data CD
	{0x0F12, 0x03FF},

	{0x0028, 0xD000},
	{0x002A, 0x1084},	// Data CD
	{0x0F12, 0x03FF},


    	{SEQUENCE_WAIT_MS, 0x0064},
	{SEQUENCE_END, 0x0000}

};

const i2c_cam_reg16 S5k5bafx_change_from_crop_to_full[] =
{
	//* Capture Cropping Code *//
	{0x0028, 0x7000},
	{0x002A, 0x0212},
	{0x0F12, 0x0640}, //#REG_TC_GP_CapZoomReqInputWidth
	{0x0F12, 0x04B0}, //#REG_TC_GP_CapZoomReqInputHeight
	{0x0F12, 0x0000}, //#REG_TC_GP_CapZoomReqInputWidthOfs
	{0x0F12, 0x0000}, //#REG_TC_GP_CapZoomReqInputHeightOfs

	//* Capture Resizing Code *//
	{0x002A, 0x0304},
	{0x0F12, 0x0640}, //#REG_0TC_CCFG_usWidth : capture output width
	{0x0F12, 0x04B0}, //#REG_0TC_CCFG_usHeight : capture output height

	//* Preview Cropping Code *//
	{0x002A, 0x020A},
	{0x0F12, 0x0640}, //#REG_TC_GP_PrevZoomReqInputWidth
	{0x0F12, 0x04B0}, //#REG_TC_GP_PrevZoomReqInputHeight
	{0x0F12, 0x0000}, //#REG_TC_GP_PrevZoomReqInputWidthOfs
	{0x0F12, 0x0000}, //#REG_TC_GP_PrevZoomReqInputHeightOfs

	//* Preview Resizing Code *//
	{0x002A, 0x0242},
	{0x0F12, 0x0280}, //0x0140 // #REG_0TC_PCFG_usWidth : preview output width
	{0x0F12, 0x01E0}, //0x00F0// #REG_0TC_PCFG_usHeight : preview output height

	//* Zoom Update *//
	{0x002A, 0x021A},
	{0x0F12, 0x0001}, // #REG_TC_GP_Inputs_ChangeRequest : 0x021A

	//* Capture Configuration Update *//
	{0x002A, 0x01F8},
	{0x0F12, 0x0001}, //REG_TC_GP_NewConfigSync 0001 : update configuration
	{0x002a, 0x0226},
	{0x0f12, 0x0001}, //REG_TC_GP_CapConfigChanged 0001

	{SEQUENCE_WAIT_MS, 0x0064},
	{SEQUENCE_END, 0x0000}

};

const i2c_cam_reg16 S5k5bafx_Preview[] =
{
	/* MaxAnGain Set */
	{0x0028, 0x7000},
	{0x002A, 0x0496},
	{0x0F12, 0x0700}, //#lt_uMaxAnGain2	: x5.5

	/* Configuration Switch */
	{0x002A, 0x021C},
	{0x0F12, 0x0000}, //#REG_TC_GP_ActivePrevConfig	  //preview config0
	{0x002A, 0x0220},
	{0x0F12, 0x0001}, //#REG_TC_GP_PrevOpenAfterChange  //config 변경적용

	/* Preview Configuration Update */
	{0x002A, 0x01F8},
	{0x0F12, 0x0001}, //REG_TC_GP_NewConfigSync 0001 : update configuration
	{0x002A, 0x021E},
	{0x0F12, 0x0001}, //#REG_TC_GP_PrevConfigChanged 0001

	{SEQUENCE_WAIT_MS, 0x0064},// Delay 100
	{SEQUENCE_END, 0x0000}

};

const i2c_cam_reg16 S5k5bafx_Capture[] =
{

	/* MaxAnGain Set */
	{0x0028, 0x7000},
	{0x002A, 0x0496},
	{0x0F12, 0x0300}, //#lt_uMaxAnGain2	:

	/* Configuration Switch */
	{0x002A, 0x0224},
	{0x0F12, 0x0000}, //#REG_TC_GP_ActiveCapConfig	//select configuration table

	/* Configuration Update */
	{0x002A, 0x01F8},
	{0x0F12, 0x0001}, //REG_TC_GP_NewConfigSync 0001 : update configuration
	{0x002A, 0x0226},
	{0x0F12, 0x0001}, //REG_TC_GP_CapConfigChanged 0001

	//{0xFFFE, 0x0100},// Delay 150
	//camera_timed_wait(150},

	/* Performing Capture */
	{0x002A, 0x01F4},
	{0x0F12, 0x0001}, //#REG_TC_GP_EnableCapture//capture
	{0x0F12, 0x0001}, //#REG_TC_GP_EnableCaptureChanged // apply
	{SEQUENCE_WAIT_MS, 0x0096},// Delay 150

	{SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_Color_Effect_None[] =
{
	{0x0028, 0x7000},
	{0x002A, 0x01EE},
	{0x0F12, 0x0000},
	{SEQUENCE_END, 0x0000}

};

const i2c_cam_reg16 S5k5bafx_Color_Effect_MONO[] =
{
	{0x0028, 0x7000},
	{0x002A, 0x01EE},
	{0x0F12, 0x0001},
	{SEQUENCE_END, 0x0000}

};


const i2c_cam_reg16 S5k5bafx_Color_Effect_NEGATIVE[] =
{
	{0x0028, 0x7000},
	{0x002A, 0x01EE},
	{0x0F12, 0x0002},
	{SEQUENCE_END, 0x0000}

};


const i2c_cam_reg16 S5k5bafx_Color_Effect_SEPIA[] =
{
	{0x0028, 0x7000},
	{0x002A, 0x01EE},
	{0x0F12, 0x0003},
	{SEQUENCE_END, 0x0000}

};

const i2c_cam_reg16 S5k5bafx_Color_Effect_Aqua[] =
{
	{0x0028, 0x7000},
	{0x002A, 0x01EE},
	{0x0F12, 0x0004},
	{SEQUENCE_END, 0x0000}

};

const i2c_cam_reg16 S5k5bafx_Color_Effect_Sketch[] =
{
	{0x0028, 0x7000},
	{0x002A, 0x01EE},
	{0x0F12, 0x0005},
	{SEQUENCE_END, 0x0000}

};


const i2c_cam_reg16 S5k5bafx_MWB_Auto[] =
{
	{0x0028, 0x7000},
	{0x002A, 0x03F8},
	{0x0F12, 0x007F},
	{SEQUENCE_END, 0x0000}

};


const i2c_cam_reg16 S5k5bafx_MWB_Incandescent[] =
{
	/* AWB off */
	{0x0028, 0x7000},
	{0x002A, 0x03F8},
	{0x0F12, 0x0077},
	/* Gain set */
	{0x002A, 0x03C6},
	{0x0F12, 0x0433},
	{0x002A, 0x03CA},
	{0x0F12, 0x0400},
	{0x002A, 0x03CE},
	{0x0F12, 0x08F0},
	/* Gain set */
	{0x002A, 0x03D2},
	{0x0F12, 0x0001},

	{SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_MWB_Fluorescent[] =
{
	/* AWB off */
	{0x0028, 0x7000},
	{0x002A, 0x03F8},
	{0x0F12, 0x0077},
	/* Gain set */
	{0x002A, 0x03C6},
	{0x0F12, 0x04F0},
	{0x002A, 0x03CA},
	{0x0F12, 0x0400},
	{0x002A, 0x03CE},
	{0x0F12, 0x0700},
	/* Gain set */
	{0x002A, 0x03D2},
	{0x0F12, 0x0001},

	{SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_MWB_Daylight[] =
{
	/* AWB off */
	{0x0028, 0x7000},
	{0x002A, 0x03F8},
	{0x0F12, 0x0077},
	/* Gain set */
	{0x002A, 0x03C6},
	{0x0F12, 0x05D0},
	{0x002A, 0x03CA},
	{0x0F12, 0x0400},
	{0x002A, 0x03CE},
	{0x0F12, 0x0540},
	/* Gain set */
	{0x002A, 0x03D2},
	{0x0F12, 0x0001},

	{SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_MWB_CloudyDaylight[] =
{
	/* AWB off */
	{0x0028, 0x7000},
	{0x002A, 0x03F8},
	{0x0F12, 0x0077},
	/* Gain set */
	{0x002A, 0x03C6},
	{0x0F12, 0x06B0},
	{0x002A, 0x03CA},
	{0x0F12, 0x0400},
	{0x002A, 0x03CE},
	{0x0F12, 0x0470},
	/* Gain set */
	{0x002A, 0x03D2},
	{0x0F12, 0x0001},

	{SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_Exposure_Minus_1[] =
{
	{0x0028, 0x7000},
	{0x002A, 0x01E4},
	{0x0F12, 0xFFE8},

	{SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_Exposure_Minus_2[] =
{
	{0x0028, 0x7000},
	{0x002A, 0x01E4},
	{0x0F12, 0xFFD0},

	{SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_Exposure_Normal[] =
{
	{0x0028, 0x7000},
	{0x002A, 0x01E4},
	{0x0F12, 0x0000},

	{SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_Exposure_Plus_1[] =
{
	{0x0028, 0x7000},
	{0x002A, 0x01E4},
	{0x0F12, 0x0018},

	{SEQUENCE_END, 0x0000}
};

const i2c_cam_reg16 S5k5bafx_Exposure_Plus_2[] =
{
	{0x0028, 0x7000},
	{0x002A, 0x01E4},
	{0x0F12, 0x0030},

	{SEQUENCE_END, 0x0000}
};

