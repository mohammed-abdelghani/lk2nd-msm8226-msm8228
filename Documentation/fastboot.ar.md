# أوامر fastboot في lk2nd
[English](fastboot.md)

يتضمن lk2nd تنفيذ fastboot ويوفّر مجموعة من الأوامر الإضافية
للتصحيح والتطوير.

> [!NOTE]
> قد لا تكون جميع أوامر fastboot مفعّلة في نسخة معينة من lk2nd.
> استخدم `fastboot oem help` لمعرفة الأوامر المتاحة.

- `oem battery` - عرض جهد البطارية ونسبة الشحن وحالتها.
- `oem dtb` - تجهيز (stage) dtb.
- `oem hash` - تجزئة البيانات المجهزة باستخدام التشفير العتادي.
- `oem log` - تجهيز سجل lk.
- `oem reboot-edl` - إعادة التشغيل إلى وضع EDL.
- `oem screenshot` - تجهيز لقطة شاشة.
- `oem debug cpuid` - تفريغ سجلات CPUID.
- `oem debug (read|write)(b|hw|l|q|pmic)` - قراءة/كتابة الذاكرة.
- `oem debug spmi-regulators` - تفريغ حالة المنظّمات (regulators).
