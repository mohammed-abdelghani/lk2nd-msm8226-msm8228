# عملية إقلاع lk2nd
[English](boot.md)

يوفّر lk2nd عدة طرق للإقلاع، لكل منها مزايا وقيود.

## Android boot.img

lk2nd مبني على CAF lk ويتضمن تنفيذ aboot. يمكنه إقلاع صورة إقلاع Android
مخزنة بإزاحة 512k من بداية قسم `boot` (في lk2nd) أو بدون الإزاحة
(في lk1st). الإزاحة مطلوبة حتى لا تستبدل صورة المرحلة التالية
lk2nd نفسه.

## الإقلاع عبر Fastboot

يوفّر lk2nd واجهة fastboot ويتيح إقلاع نظام تشغيل عبر USB. مثل الخيار
السابق، هذا يأتي من CAF lk ويستخدم صور إقلاع Android.

```
fastboot boot boot.img
```

## extlinux.conf

يوفّر lk2nd دعمًا بدائيًا جدًا لملف `extlinux.conf`، مشابهًا لـ
u-boot وdepthcharge.

يجب أن يُخزّن الملف في قسم مُنسّق بـ `ext2` على المسار `/extlinux/extlinux.conf`.
سيبحث lk2nd عنه في أي قسم أكبر من 16MiB وفي قسم `boot`
(بإزاحة 512k في lk2nd).

الأوامر المدعومة:

- `label <label>`       - بدء إدخال إقلاع جديد.
- `default <label>`     - تعيين التسمية (label) المستخدمة للإقلاع.
- `linux <kernel>`      - المسار إلى صورة النواة. (بديل: `kernel`)
- `initrd <initramfs>`  - المسار إلى ملف initramfs.
- `fdt <devicetree>`    - المسار إلى شجرة الأجهزة. (بديل: `devicetree`)
- `fdtdir <directory>`  - المسار للعثور على DT تلقائيًا. (بديل: `devicetreedir`)
- `append <cmdline>`    - سطر أوامر النواة للإقلاع.
- `fdtoverlays <files>` - قائمة مفصولة بمسافات من تراكبات (overlays) DT للتطبيق. (بديل: `devicetree-overlay`)

> [!NOTE]
> يتضمن lk2nd حاليًا دعمًا بدائيًا جدًا لـ extlinux.
> فقط الأوامر المدرجة أعلاه مُعتمدة، وسيُقلع lk2nd دائمًا
> التسمية "default".

### مثال على extlinux.conf

المثال أدناه يعرض `extlinux.conf` «صحيحًا» يتضمن توجيهات إضافية
غير مدعومة في lk2nd. سيتجاهل lk2nd الأوامر غير المعروفة.

```
timeout 1
menu title Boot the OS
default MyOS

# Bootloader should pick the DT
label MyOS
    linux /vmlinuz
    initrd /initramfs
    fdtdir /dtbs
    append earlycon console=ttyMSM0,115200
```

### تخطيط ذاكرة الإقلاع مع extlinux.conf

عند الإقلاع عبر `extlinux.conf`، يحتاج lk2nd إلى اختيار عناوين بعد
النواة لتوفير initramfs وشجرة الأجهزة.

ذاكرة الإقلاع المتاحة محدودة بمناطق الذاكرة المحجوزة القريبة وتختلف
حسب المنصة. يمكن للمنصات تعيين `LK2ND_BOOT_MEM_SIZE` لتعديل الحجم.
سيستخدم lk2nd:
- `msm8960`: 128 ميغابايت
- جميع المنصات الأخرى: 50 ميغابايت

تلك الذاكرة مشتركة بين النواة وinitramfs وشجرة الأجهزة. سيفشل
الإقلاع إذا تجاوز هذا الحد. الذاكرة المطلوبة من النواة تشمل الحجم
بعد فك الضغط الذاتي، وليس فقط الحجم على القرص للنواة. نوى ARM
32-bit والنوى القديمة 64-bit لا توفر تفاصيل عن مقدار الذاكرة
التي تحتاجها لفك الضغط الذاتي، لذا يضع lk2nd الـ initramfs
وشجرة الأجهزة في نهاية ذاكرة الإقلاع، لإتاحة كل الذاكرة المتبقية
للنواة لفك الضغط.

## معاملات سطر أوامر lk2nd

يمكن لـ lk2nd قراءة معاملات سطر أوامر نظام التشغيل واتخاذ بعض
القرارات أثناء إقلاعه.

> [!IMPORTANT]  
> يقرأ lk2nd هذه القيم من نظام التشغيل الذي يقلعه، وليس من سطر أوامر نفسه.

- `lk2nd.pass-simplefb(=...)` - إضافة عقدة simplefb إلى dtb.
  إذا تم تعيين `autorefresh`، فسيتم تمكين التحديث التلقائي للشاشات ذات وضع الأمر (command mode).
  إذا تم تعيين `xrgb8888` أو `rgb565`، فسيتم التبديل إلى وضع العرض المحدد.
  إذا تم تعيين `relocate`، فسيتم تغيير عنوان إطار الشاشة (framebuffer) إلى منطقة كبيرة وآمنة نسبيًا.
  يمكن دمج الخيارات. (أي `...=xrgb8888,autorefresh`)
- `lk2nd.pass-ramoops(=zap)` - إضافة عقدة ramoops إلى dtb. إذا تم تعيين `zap`،
  فسيتم مسح المنطقة قبل الإقلاع. استخدم أوامر `fastboot oem ramoops ...` للحصول على البيانات.
- `lk2nd.spin-table=force` - فرض تفعيل spin table حتى لو كان PSCI متاحًا.
