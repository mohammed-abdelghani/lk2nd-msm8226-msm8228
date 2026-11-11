# بناء lk2nd
[English](building.md)

## المتطلبات
- `make`
- Python 3: `python3`
- سلسلة أدوات GCC لـ ARM (32 bit)
  - Arch Linux: `arm-none-eabi-gcc`
  - Alpine Linux و postmarketOS: `gcc-arm-none-eabi`
  - Debian و Ubuntu: `gcc-arm-none-eabi`
  - Fedora: `arm-none-eabi-gcc-cs`
- [مترجم Device Tree](https://git.kernel.org/pub/scm/utils/dtc/dtc.git)
  - Arch Linux: `dtc`
  - Alpine Linux و postmarketOS: `dtc`
  - Debian و Ubuntu: `device-tree-compiler`
  - Fedora: `dtc`
- libfdt
  - Alpine Linux و postmarketOS: `dtc-dev`
  - Debian و Ubuntu: `libfdt-dev`
  - Fedora: `libfdt-devel`
- GNU tar
  - Alpine Linux و postmarketOS: `tar`
- متطلبات اختيارية عند استخدام توقيع الصور
  - Arch Linux: `python-pyasn1-modules python-pycryptodome`
  - Alpine Linux و postmarketOS: `py3-asn1-modules py3-pycryptodome`
  - Debian و Ubuntu: `python3-pyasn1-modules python3-pycryptodome`
  - Fedora: `python3-pyasn1-modules python3-pycryptodomex`

## بناء lk2nd

راجع targets.md لمعرفة هدف الـ make الذي يجب استخدامه أدناه.
(يعتمد ذلك على رقاقة النظام في جهازك.)

```
$ make TOOLCHAIN_PREFIX=arm-none-eabi- lk2nd-msmXXXX
```

استبدل `TOOLCHAIN_PREFIX` بالمسار إلى سلسلة أدواتك.
تُبنى `lk2nd.img` وتُوضع في `build-lk2nd-msmXXXX/lk2nd.img`.

## بناء lk1st

**ملاحظة:** على عكس lk2nd، لا يزال lk1st تجريبيًا ولم يُوصف
هنا بعد.

## أعلام البناء الإضافية

يوفّر نظام بناء lk2nd بعض إعدادات وقت الترجمة الإضافية التي يمكنك إضافتها
إلى سطر أوامر `make` لتفعيل بعض الميزات الإضافية.

### الإعدادات العامة

#### `DEBUG=` - مستوى السجل

عيّنه إلى 0 لقمع معظم رسائل السجل، وعيّنه إلى 2 لتفعيل رسائل سجل كثيفة
(قد يُبطئ الإقلاع). القيمة الافتراضية هي 1.

#### `DEBUG_FBCON=` - تفعيل التسجيل على الشاشة

عيّنه إلى 1 لجعل lk2nd يطبع السجلات على الشاشة.

#### `DEBUG_UART_ID=` - تكوين نسخة UART المستخدمة لمخرجات التصحيح

حدّد أي نسخة UART في الرقاقة لتسجيل رسائل التصحيح. حاليًا تدعم فقط
نسخ UART محددة:

  - `mdm9607`: 2, 5

#### `LK2ND_VERSION=` - تجاوز سلسلة إصدار lk2nd

افتراضيًا، يحاول نظام بناء lk2nd الحصول على الإصدار من git. إذا احتجت
لتجاوز الإصدار (مثلاً إذا أردت تغليف بناء lk2nd)، عيّن هذا المتغير.

#### `LK2ND_FORCE_FASTBOOT=` - إجبار lk2nd على الإقلاع إلى قائمة fastboot

بتعيين هذا الخيار إلى 1، سيدخل lk2nd دائمًا القائمة عند الإقلاع بدلًا من
المتابعة بسير العمل المعتاد. هذا مفيد للتصحيح والتطوير.

#### `LK2ND_FASTBOOT_DELAY=` - إضافة تأخير قبل الإقلاع عبر `fastboot boot` (بالميلي ثانية)

يمكن أن يساعد هذا في التصحيح على الأجهزة ذات UART الكابل.
عليك تبديل الكابل قبل بدء Linux لترى جميع السجلات.

#### `PANIC_REBOOT_MODE=` - تكوين وضع إعادة التشغيل بعد الانهيارات

يحاول LK إعادة التشغيل عند حدوث خطأ فادح (panic) أثناء التشغيل. يتيح هذا
الخيار تغيير الوضع الذي يحاول الجهاز إعادة التشغيل إليه. لاحظ أنه بسبب
اختلافات البرمجيات الثابتة بين المورّدين المختلفين، قد يُشغّل هذا الخيار
إعادة تشغيل عادية فقط، دون الدخول إلى وضع fastboot/download.

الافتراضي يعتمد على تكوين المشروع:
- **lk2nd:** `FASTBOOT_MODE`
- **lk1st:** `EMERGENCY_DLOAD`
- **Stock:** `NORMAL_DLOAD`
- خيارات أخرى: `NO_REBOOT`, `NORMAL_REBOOT`, `RECOVERY_MODE`

### إعدادات خاصة بـ lk2nd

#### `LK2ND_ADTBS=`, `LK2ND_QCDTBS=`, `LK2ND_DTBS=` - بناء dtbs المدرجة فقط

#### `LK2ND_BOOT_MEM_SIZE=` - أقصى حجم لذاكرة الإقلاع بالبايتات

عيّن الحد الأقصى للذاكرة المتاحة للإقلاع (النواة وشجرة الأجهزة
و initramfs). القيمة الافتراضية هي `0x03200000` (50 ميغابايت).

### إعدادات خاصة بـ lk1st

#### `LK2ND_COMPATIBLE=` - توافق اللوحة

عيّن قيمة توافق اللوحة.

#### `LK2ND_BUNDLE_DTB=` - ملف dtb الخاص باللوحة

استخدم هذا dtb لبناء lk1st.

#### `LK2ND_DISPLAY=` - تعريف لوحة العرض

عيّن تعريف لوحة محدد. افتراضيًا يستخدم `cont-splash`.

### توقيع الصور

#### `SIGN_BOOTIMG=` - توقيع `lk2nd.img` بعد البناء

عيّنه إلى 1 لتوقيع `lk2nd.img` الناتجة لـ AVB1. افتراضيًا
لا يتم توقيع الصور.

#### `BOOTIMG_CERT=` و `BOOTIMG_KEY=` - تعيين بيانات الاعتماد المستخدمة في التوقيع

عيّن شهادة التوقيع والمفتاح الخاص المستخدمين لتوقيع صور الإقلاع.
إذا كان التوقيع مفعّلًا ولم يتم تحديد مفاتيح، تُستخدم مجموعة افتراضية
من مفاتيح اختبار من شجرة مصادر AOSP 13 الموجودة في `lk2nd/certs/`
لتوقيع الصورة.
`BOOTIMG_CERT` يُتوقع أن يكون بصيغة X.509 PEM و `BOOTIMG_KEY` يُتوقع
أن يكون بصيغة PKCS#8.
