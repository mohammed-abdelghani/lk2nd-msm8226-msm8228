# Supported devices
[العربية](devices.ar.md)


### lk2nd-msm8226
- ASUS ZenWatch 2 - sparrow
- Coolpad 8730L - 8730l
- HTC One Mini 2 - memul 
- Huawei Ascend G6 4G - G6-L11 
- Huawei Ascend G6 - G6-L22 
- Huawei Watch - sturgeon
- LG Fx0 - madai
- LG G Watch R - lenok
- Motorola Moto G 2013 - falcon
- Motorola Moto G 4G 2013 - peregrine
- Motorola Moto G 2014 - titan
- Motorola Moto G 2014 LTE - thea
- Samsung Galaxy Grand 2 - SM-G7102
- Samsung Galaxy Tab 4 10.1 (2014) - SM-T530, SM-T531, SM-T532, SM-T535
- Samsung Galaxy Tab 4 8.0 (2014) - SM-T330, SM-T330NU, SM-T331
- Sony Xperia M2 - eagle 

### lk2nd-msm8228
- Samsung Galaxy Note 3 Neo Dos - SM-N7502

## Minimal DTB Overlay

The newest devices supported by lk2nd (particularly SDM429/SDM439/SDM632 ones)
require the installation of a custom, minimal DTBO partition image. Otherwise,
the boot loader would try to patch lk2nd's device trees with the stock overlay
which references all the nodes that don't exist anymore, and refuses to proceed.


## Porting new devices

If you are considering adding support for a new device, you would need to add a
new dts entry. You can see [targets.md](targets.md) to pick the correct target
for your SoC and consult [dt-bindings.md](dt-bindings.md) for the lk2nd dts format
and bindings.
