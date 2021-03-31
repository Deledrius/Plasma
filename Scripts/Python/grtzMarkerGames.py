# -*- coding: utf-8 -*-
""" *==LICENSE==*

CyanWorlds.com Engine - MMOG client, server and tools
Copyright (C) 2011  Cyan Worlds, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Additional permissions under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or
combining it with any of RAD Game Tools Bink SDK, Autodesk 3ds Max SDK,
NVIDIA PhysX SDK, Microsoft DirectX SDK, OpenSSL library, Independent
JPEG Group JPEG library, Microsoft Windows Media SDK, or Apple QuickTime SDK
(or a modified version of those libraries),
containing parts covered by the terms of the Bink SDK EULA, 3ds Max EULA,
PhysX SDK EULA, DirectX SDK EULA, OpenSSL and SSLeay licenses, IJG
JPEG Library README, Windows Media SDK EULA, or QuickTime SDK EULA, the
licensors of this Program grant you additional
permission to convey the resulting work. Corresponding Source for a
non-source form of such a combination shall include the source code for
the parts of OpenSSL and IJG JPEG Library used as well as that of the covered
work.

You can contact Cyan Worlds, Inc. by email legal@cyan.com
 or by snail mail at:
      Cyan Worlds, Inc.
      14617 N Newport Hwy
      Mead, WA   99021

 *==LICENSE==* """

from Plasma import *

MG01 = (
    ("city", ptPoint3(0.113123, -156.593, 221.15), "3, 53, -80"),
    ("city", ptPoint3(17.0199, -315.718, 144.806), "62334, 63, -85"),
    ("city", ptPoint3(6.97063, -312.509, 131.561), "62432, 63, -86"),
    ("city", ptPoint3(40.9337, -437.902, 82.8586), "62143, 71, -89"),
    ("city", ptPoint3(-142.135, -634.978, 8.21551), "32257, 84, -93"),
    ("city", ptPoint3(97.9118, -555.991, 6.74272), "61728, 78, -93"),
    ("city", ptPoint3(146.221, -571.129, 9.02309), "61363, 80, -93"),
    ("city", ptPoint3(62.3171, -496.638, 6.83943), "61984, 75, -93"),
    ("city", ptPoint3(95.1365, -14.2289, 243.928), "61188, 45, -79"),
    ("city", ptPoint3(198.983, 27.1469, 291.171), "59651, 44, -76"),
    ("city", ptPoint3(154.969, 4.03381, 290.841), "60329, 44, -76"),
    ("city", ptPoint3(-0.621237, -329.797, 135.381), "9, 64, -85"),
)

MG02 = (
    ("city", ptPoint3(866.687, -645.153, 238.134), "56815, 100, -79"),
    ("city", ptPoint3(884.89, -722.404, 232.785), "56972, 104, -79"),
    ("city", ptPoint3(840.865, -645.9, 244.26), "56953, 99, -79"),
    ("city", ptPoint3(164.1, -408.321, 162.822), "61041, 70, -84"),
    ("city", ptPoint3(-130.384, -284.278, 127.039), "32495, 62, -86"),
    ("city", ptPoint3(-128.733, -283.422, 127.041), "32479, 62, -86"),
    ("city", ptPoint3(-126.874, -283.311, 127.042), "32461, 62, -86"),
    ("city", ptPoint3(-125.991, -283.299, 127.043), "32452, 62, -86"),
    ("city", ptPoint3(-90.4871, -233.48, 127.11), "32143, 58, -86"),
    ("city", ptPoint3(-112.979, -239.741, 127.444), "32372, 59, -86"),
    ("city", ptPoint3(-121.025, -244.955, 131.027), "32449, 59, -86"),
    ("city", ptPoint3(-147.008, -248.051, 127.11), "32712, 60, -86"),
    ("city", ptPoint3(-199.518, 19.7047, 225.826), "33992, 44, -80"),
    ("city", ptPoint3(-199.09, 24.6076, 225.826), "34005, 44, -80"),
    ("city", ptPoint3(-193.792, 30.2368, 225.826), "33954, 43, -80"),
    ("city", ptPoint3(-234.291, 47.5248, 253.739), "34580, 43, -78"),
    ("city", ptPoint3(-290.803, 25.9652, 254.499), "35207, 45, -78"),
    ("city", ptPoint3(-245.232, 104.78, 254.478), "35030, 40, -78"),
    ("city", ptPoint3(173.349, -349.866, 180.313), "60876, 66, -83"),
    ("city", ptPoint3(841.677, -644.558, 244.414), "56945, 99, -79"),
    ("city", ptPoint3(179.525, -354.162, 180.313), "60826, 66, -83"),
    ("city", ptPoint3(169.036, -519.697, 160.798), "61133, 77, -84"),
    ("city", ptPoint3(839.212, -648.525, 243.975), "56971, 99, -79"),
    ("city", ptPoint3(845.905, -690.159, 209.033), "57070, 101, -81"),
)

MG03 = (
    ("city", ptPoint3(779.755, -587.08, 259.243), "57090, 94, -78"),
    ("city", ptPoint3(454.014, -382.702, 297.221), "58559, 73, -75"),
    ("city", ptPoint3(873.06, -808.26, 259.243), "57287, 109, -78"),
    ("city", ptPoint3(966.86, -801.529, 259.254), "56817, 111, -78"),
    ("city", ptPoint3(885.322, -770.273, 259.243), "57116, 107, -78"),
    ("city", ptPoint3(898.188, -768.307, 269.941), "57046, 107, -77"),
    ("city", ptPoint3(972.309, -712.254, 269.975), "56510, 107, -77"),
    ("city", ptPoint3(867.876, -607.192, 269.983), "56679, 98, -77"),
    ("city", ptPoint3(806.465, -662.197, 269.863), "57191, 99, -77"),
    ("city", ptPoint3(675.79, -518.872, 256.664), "57473, 87, -78"),
    ("city", ptPoint3(491.578, -324.465, 251.259), "58059, 71, -78"),
)

MG04 = (
    ("city", ptPoint3(262.733, -532.497, 239.805), "60415, 78, -79"),
    ("city", ptPoint3(225.478, -513.173, 216.521), "60676, 77, -80"),
    ("city", ptPoint3(-135.211, -637.171, 10.4902), "32205, 84, -93"),
    ("city", ptPoint3(881.076, -803.77, 259.244), "57235, 109, -78"),
    ("city", ptPoint3(877.444, -803.838, 259.244), "57253, 108, -78"),
    ("city", ptPoint3(870.754, -803.964, 259.244), "57287, 108, -78"),
    ("city", ptPoint3(840.792, -804.53, 246.271), "57438, 107, -79"),
    ("city", ptPoint3(-0.0876787, -314.3, 144.145), "5, 63, -85"),
    ("city", ptPoint3(13.9271, -154.523, 224.159), "62339, 53, -80"),
    ("city", ptPoint3(-59.7848, -110.308, 221.673), "31905, 50, -80"),
    ("city", ptPoint3(-118.658, -163.122, 221.597), "32534, 54, -80"),
    ("city", ptPoint3(-146.071, -180.047, 218.473), "32813, 55, -80"),
    ("city", ptPoint3(-215.992, -225.902, 186.949), "33459, 59, -82"),
    ("city", ptPoint3(-197.099, -213.275, 194.361), "33293, 58, -82"),
    ("city", ptPoint3(-165.729, -195.325, 220.011), "32998, 57, -80"),
    ("city", ptPoint3(75.1814, -125.345, 223.846), "61600, 51, -80"),
    ("city", ptPoint3(167.221, -328.274, 178.47), "60901, 65, -83"),
    ("city", ptPoint3(238.621, -351.974, 181.112), "60287, 67, -83"),
    ("city", ptPoint3(296.351, -428.617, 178.973), "59951, 73, -83"),
    ("city", ptPoint3(323.009, -503.966, 244.604), "59898, 78, -79"),
)

MG05 = (
    ("city", ptPoint3(152.191, 21.0605, 293.854), "60315, 43, -76"),
    ("city", ptPoint3(383.555, -111.711, 257.587), "58121, 56, -78"),
    ("city", ptPoint3(343.068, 16.5379, 292.971), "57888, 47, -76"),
    ("city", ptPoint3(318.848, 41.6997, 291.357), "58029, 45, -76"),
    ("city", ptPoint3(40.6364, -152.596, 224.379), "62028, 53, -80"),
    ("city", ptPoint3(100.535, -247.86, 193.841), "61452, 59, -82"),
    ("city", ptPoint3(782.075, -583.084, 259.243), "57064, 94, -78"),
    ("city", ptPoint3(704.187, -510.593, 259.243), "57268, 87, -78"),
    ("city", ptPoint3(779.124, -436.175, 296.512), "56531, 86, -75"),
    ("city", ptPoint3(798.406, -367.264, 297.593), "56122, 83, -75"),
    ("city", ptPoint3(672.346, -335.633, 297.594), "56783, 77, -75"),
    ("city", ptPoint3(521.539, -233.122, 294.92), "57442, 66, -75"),
    ("city", ptPoint3(431.085, -401.132, 294.896), "58796, 74, -75"),
    ("city", ptPoint3(399.419, -467.361, 299.302), "59227, 77, -75"),
    ("city", ptPoint3(416.217, -547.247, 277.85), "59302, 82, -77"),
    ("city", ptPoint3(370.373, -565.982, 239.353), "59674, 82, -79"),
    ("city", ptPoint3(306.196, -485.725, 243.624), "59992, 76, -79"),
    ("city", ptPoint3(262.047, -532.807, 237.377), "60421, 78, -79"),
    ("city", ptPoint3(234.399, -468.967, 208.197), "60536, 74, -81"),
    ("city", ptPoint3(261.147, -452.115, 206.569), "60288, 73, -81"),
    ("city", ptPoint3(298.73, -431.849, 178.966), "59939, 73, -83"),
    ("city", ptPoint3(230.511, -343.587, 178.72), "60343, 66, -83"),
    ("city", ptPoint3(191.976, -327.469, 178.508), "60667, 65, -83"),
    ("city", ptPoint3(68.7406, -40.7315, 230.585), "61584, 46, -80"),
    ("city", ptPoint3(244.142, 67.786, 291.356), "58848, 42, -76"),
    ("city", ptPoint3(332.792, 58.5213, 291.296), "57756, 45, -76"),
    ("city", ptPoint3(349.096, -67.2592, 274.02), "58265, 52, -77"),
    ("city", ptPoint3(462.965, -72.0451, 297.591), "57142, 56, -75"),
)

MG06 = (
    ("Neighborhood", ptPoint3(275.215, -713.491, 19.1162), "60283, 1005, -70"),
    ("Neighborhood", ptPoint3(261.281, -750.158, 21.5005), "60270, 1003, -70"),
    ("Neighborhood", ptPoint3(213.126, -767.487, 9.8439), "60238, 1003, -71"),
    ("Neighborhood", ptPoint3(172.793, -775.21, 3.32227), "60213, 1003, -71"),
    ("Neighborhood", ptPoint3(280.228, -796.492, 9.01678), "60275, 1000, -71"),
    ("Neighborhood", ptPoint3(226.41, -854.221, 1.65846), "60234, 998, -71"),
    ("Neighborhood", ptPoint3(123.435, -931.588, -19.6041), "60160, 994, -73"),
    ("Neighborhood", ptPoint3(186.016, -792.772, -18.2181), "60218, 1002, -73"),
    ("Neighborhood", ptPoint3(160.845, -750.635, -10.8941), "60209, 1005, -72"),
    ("Neighborhood", ptPoint3(305.288, -706.162, 21.0971), "60302, 1005, -70"),
    ("Neighborhood", ptPoint3(354.996, -742.071, 19.1236), "60327, 1003, -70"),
    ("Neighborhood", ptPoint3(324.906, -766.67, 19.1239), "60306, 1002, -70"),
    ("Neighborhood", ptPoint3(195.939, -835.297, 5.82845), "60218, 999, -71"),
    ("Neighborhood", ptPoint3(135.273, -767.306, 11.7715), "60191, 1004, -71"),
)



MG07 = (
    ("Neighborhood", ptPoint3(0.380994, -771.841, 8.10068), "60109, 1006, -71"),
    ("Neighborhood", ptPoint3(59.4292, -782.611, 8.15849), "60143, 1004, -71"),
    ("Neighborhood", ptPoint3(-11.4838, -788.725, 8.10068), "60100, 1005, -71"),
    ("Neighborhood", ptPoint3(-20.6219, -775.695, 8.10068), "60096, 1006, -71"),
    ("Neighborhood", ptPoint3(-25.0077, -763.53, 8.10068), "60095, 1007, -71"),
    ("Neighborhood", ptPoint3(84.2148, -804.868, 15.692), "60155, 1003, -71"),
    ("Neighborhood", ptPoint3(31.4565, -796.199, 81.4172), "60125, 1004, -66"),
    ("Neighborhood", ptPoint3(214.071, -873.879, -18.5609), "60224, 997, -73"),
    ("Neighborhood", ptPoint3(240.256, -759.147, 9.85464), "60256, 1003, -71"),
    ("Neighborhood", ptPoint3(280.289, -857.252, 5.71199), "60266, 997, -71"),
    ("Neighborhood", ptPoint3(55.9215, -780.317, 29.2447), "60142, 1005, -70"),
    ("Neighborhood", ptPoint3(107.642, -735.816, 7.56582), "60179, 1006, -71"),
    ("Neighborhood", ptPoint3(137.585, -766.796, 12.0827), "60193, 1004, -71"),
    ("Neighborhood", ptPoint3(182.913, -774.13, 10.5281), "60219, 1003, -71"),
    ("Neighborhood", ptPoint3(250.806, -779.415, 9.80428), "60259, 1002, -71"),
    ("Neighborhood", ptPoint3(297.308, -783.464, 9.14582), "60287, 1001, -71"),
)

MG08 = (
    ("city", ptPoint3(316.219, -514.793, 237.373), "59972, 78, -79"),
    ("city", ptPoint3(365.981, -563.018, 237.389), "59700, 82, -79"),
    ("city", ptPoint3(415.82, -538.035, 274.685), "59283, 81, -77"),
    ("city", ptPoint3(425.815, -416.746, 294.899), "58884, 74, -75"),
    ("city", ptPoint3(469.789, -341.995, 294.9), "58296, 71, -75"),
    ("city", ptPoint3(467.679, -330.837, 294.91), "58273, 70, -75"),
    ("city", ptPoint3(465.28, -318.14, 268.098), "58246, 70, -77"),
    ("city", ptPoint3(260.434, -160.43, 260.027), "59585, 56, -78"),
    ("city", ptPoint3(214.6, -160.99, 259.761), "60078, 55, -78"),
    ("city", ptPoint3(129.821, -176.251, 215.822), "61042, 55, -80"),
    ("city", ptPoint3(189.439, -327.706, 178.497), "60692, 65, -83"),
    ("city", ptPoint3(180.853, -314.042, 177.981), "60749, 64, -83"),
    ("city", ptPoint3(245.511, -333.635, 150.648), "60186, 66, -84"),
    ("city", ptPoint3(65.3244, -127.249, 221.673), "61719, 52, -80"),
    ("city", ptPoint3(-2.39575, -13.6359, 220.941), "35, 44, -80"),
    ("city", ptPoint3(-42.9333, -145.408, 277.339), "31681, 53, -77"),
    ("city", ptPoint3(-54.6834, -191.584, 278.654), "31789, 56, -76"),
    ("city", ptPoint3(-71.0139, -181.946, 274.244), "31978, 55, -77"),
    ("city", ptPoint3(-68.8656, -167.067, 284.985), "31966, 54, -76"),
    ("city", ptPoint3(-42.0513, -195.511, 277.165), "467, 56, -77"),
)

MG09 = (
    ("city", ptPoint3(1.77311, -143.133, 220.935), "62480, 52, -80"),
    ("city", ptPoint3(-47.6192, -128.094, 221.671), "31746, 52, -80"),
    ("city", ptPoint3(-133.881, -194.159, 221.659), "32656, 56, -80"),
    ("city", ptPoint3(-36.892, -77.407, 221.679), "31642, 48, -80"),
    ("city", ptPoint3(-182.53, 15.0192, 221.679), "33745, 44, -80"),
    ("city", ptPoint3(-11.8993, -39.2203, 220.12), "161, 46, -80"),
    ("city", ptPoint3(-13.1011, 35.0907, 220.104), "31354, 41, -80"),
    ("city", ptPoint3(32.8043, -51.3977, 221.76), "62068, 47, -80"),
    ("city", ptPoint3(50.5303, -118.34, 221.66), "61889, 51, -80"),
    ("city", ptPoint3(42.9236, -133.187, 221.661), "61990, 52, -80"),
    ("city", ptPoint3(123.175, -9.2883, 261.196), "60797, 45, -78"),
    ("city", ptPoint3(82.4942, -144.117, 222.015), "61535, 53, -80"),
    ("city", ptPoint3(70.1662, -220.908, 195.87), "61746, 57, -82"),
    ("city", ptPoint3(-4.62847, -215.158, 194.066), "51, 57, -82"),
    ("city", ptPoint3(7.58955, -205.743, 199.97), "62417, 56, -81"),
    ("city", ptPoint3(1.26902, -94.9741, 220.931), "62485, 49, -80"),
)

MG10 = (
    ("city", ptPoint3(-169.248, -599.338, 8.22528), "32489, 82, -93"),
    ("city", ptPoint3(-159.089, -572.478, 8.22528), "32437, 80, -93"),
    ("city", ptPoint3(-146.356, -536.789, 8.22528), "32370, 78, -93"),
    ("city", ptPoint3(50.8871, -483.389, 62.6932), "62074, 74, -90"),
    ("city", ptPoint3(6.66464, -425.883, 92.6597), "62442, 70, -88"),
    ("city", ptPoint3(-2.43234, -276.531, 155.824), "26, 61, -84"),
    ("city", ptPoint3(2.97976, -148.419, 223.733), "62466, 53, -80"),
    ("city", ptPoint3(61.4058, -129.401, 221.671), "61768, 52, -80"),
    ("city", ptPoint3(113.078, -162.948, 214.661), "61208, 54, -80"),
    ("city", ptPoint3(489.8, -304.099, 294.899), "57996, 69, -75"),
    ("city", ptPoint3(549.031, -170.577, 297.581), "56916, 64, -75"),
    ("city", ptPoint3(553.686, -196.203, 297.581), "57008, 65, -75"),
    ("city", ptPoint3(588.206, -227.224, 297.593), "56889, 68, -75"),
    ("city", ptPoint3(603.84, -253.754, 297.593), "56898, 70, -75"),
    ("city", ptPoint3(666.27, -332.168, 297.594), "56809, 76, -75"),
    ("city", ptPoint3(775.175, -341.96, 297.595), "56149, 81, -75"),
    ("city", ptPoint3(801.331, -397.677, 297.595), "56238, 85, -75"),
    ("city", ptPoint3(777.84, -421.14, 299.93), "56477, 85, -75"),
    ("city", ptPoint3(715.306, -504.603, 256.659), "57179, 87, -78"),
    ("city", ptPoint3(765.788, -589.046, 259.234), "57176, 93, -78"),
    ("city", ptPoint3(874.804, -556.476, 259.243), "56459, 95, -78"),
    ("city", ptPoint3(951.983, -589.045, 259.243), "56182, 100, -78"),
    ("city", ptPoint3(968.839, -630.479, 259.243), "56248, 102, -78"),
    ("city", ptPoint3(1000.34, -713.188, 259.243), "56381, 108, -78"),
    ("city", ptPoint3(1003.1, -748.014, 259.243), "56481, 110, -78"),
    ("city", ptPoint3(978.66, -745.66, 259.243), "56588, 109, -78"),
    ("city", ptPoint3(893.636, -809.511, 259.243), "57190, 109, -78"),
    ("city", ptPoint3(849.176, -824.857, 222.094), "57452, 109, -80"),
)

MG11 = (
    ("city", ptPoint3(-0.0891117, -155.874, 223.466), "3, 53, -80"),
    ("city", ptPoint3(0.0849568, -510.678, 6.83298), "3, 75, -93"),
    ("city", ptPoint3(0.046029, -468.112, 30.0315), "3, 73, -92"),
    ("city", ptPoint3(0.0126944, -425.414, 90.2389), "3, 70, -88 "),
    ("city", ptPoint3(0.0880445, -330.854, 134.733), "3, 64, -85"),
    ("city", ptPoint3(-0.0940349, -308.747, 144.135), "3, 63, -85 "),
    ("city", ptPoint3(0.107221, -82.8188, 220.945), "3, 49, -80"),
    ("city", ptPoint3(0.0100019, 31.4722, 220.945), "3, 41, -80"),
    ("GreatZero", ptPoint3(0.000921001, -25.3192, -40.0522), "0, 1, -3"),
)

MG12 = (
    ("city", ptPoint3(-76.7246, -463.087, 8.21978), "31851, 73, -93"),
    ("city", ptPoint3(-10.8075, -413.394, 92.4359), "97, 69, -88"),
    ("city", ptPoint3(-11.6655, -320.016, 133.759), "114, 63, -86 "),
    ("city", ptPoint3(-3.82988, -312.859, 144.135), "39, 63, -85"),
    ("city", ptPoint3(-107.042, -293.259, 130.295), "32254, 62, -86"),
    ("city", ptPoint3(-140.033, -212.729, 127.077), "32694, 57, -86"),
    ("city", ptPoint3(-140.033, -212.729, 274.241), "32694, 57, -77"),
    ("city", ptPoint3(0.388106, -150.975, 220.94), "62496, 53, -80 "),
    ("city", ptPoint3(69.3479, -41.5769, 221.68), "61577, 46, -80"),
)

MG13 = (
    ("GreatZero", ptPoint3(-0.107815, -48.5057, -40.0535), "22, 3, -3"),
    ("GreatZero", ptPoint3(-46.3505, 16.0544, -40.0403), "18929, 3, -3 "),
    ("GreatZero", ptPoint3(38.3031, 29.4412, -40.04), "40365, 3, -3 "),
    ("city", ptPoint3(-146.164, -481.771, 8.22064), "32420, 74, -93"),
    ("city", ptPoint3(-76.4565, -492.799, 7.64378), "31834, 74, -93 "),
    ("city", ptPoint3(-4.83725, -489.336, 6.84302), "41, 74, -93"),
    ("city", ptPoint3(83.4286, -493.755, 60.0126), "61808, 74, -90 "),
    ("city", ptPoint3(239.618, -470.852, 208.239), "60496, 74, -81"),
)

MG14 = (
    ("city", ptPoint3(467.321, -331.116, 294.91), "58277, 70, -75 "),
    ("city", ptPoint3(386.313, -177.083, 260.518), "58384, 60, -78 "),
    ("city", ptPoint3(338.75, 41.1883, 291.95), "57792, 46, -76 "),
    ("city", ptPoint3(282.086, 53.5703, 293.757), "58422, 44, -76 "),
    ("city", ptPoint3(243.691, -339.626, 157.405), "60215, 66, -84"),
    ("city", ptPoint3(173.38, -315.199, 158.217), "60822, 64, -84"),
    ("city", ptPoint3(167.893, -392.185, 181.574), "60987, 69, -83 "),
    ("city", ptPoint3(58.2999, -197.159, 209.46), "61857, 56, -81 "),
    ("city", ptPoint3(131.894, -172.566, 216.863), "61013, 55, -80"),
    ("city", ptPoint3(-41.2759, -146.405, 274.388), "31661, 53, -77 "),
    ("city", ptPoint3(128.527, -122.188, 261.352), "60963, 52, -78 "),
    ("city", ptPoint3(-3.08397, 12.4945, 223.141), "46, 43, -80 "),
    ("city", ptPoint3(-98.4153, -28.1701, 223.632), "32498, 46, -80 "),
    ("city", ptPoint3(-182.556, -170.771, 198.891), "33231, 55, -81"),
    ("city", ptPoint3(-196.998, -214.11, 196.267), "33290, 58, -82 "),
    ("city", ptPoint3(-47.6542, -61.9222, 221.684), "31789, 47, -80 "),
    ("city", ptPoint3(-68.2777, -96.0332, 221.684), "32022, 50, -80"),
    ("city", ptPoint3(-77.9, -55.3034, 221.684), "32188, 47, -80"),
    ("city", ptPoint3(-64.4581, -74.3091, 221.684), "31995, 48, -80"),
)

mgs = (MG01, MG02, MG03, MG04, MG05, MG06, MG07, MG08, MG09, MG10, MG11,
       MG12, MG13, MG14)
