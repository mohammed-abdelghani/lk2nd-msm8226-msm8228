<h1 align="center">lk2nd</h1>

<p align="center">
  محمّل إقلاع مخصص لرقائق Qualcomm MSM8228 و MSM8226
</p>

<p align="center">
  <a href="https://github.com/mohammed-abdelghani/lk2nd-msm8226-msm8228/blob/main/LICENSE"><img src="https://img.shields.io/badge/license-MIT-blue.svg" alt="MIT License"></a>
  <a href="https://github.com/mohammed-abdelghani/lk2nd-msm8226-msm8228/releases"><img src="https://img.shields.io/badge/releases-latest-orange.svg" alt="Releases"></a>
  <a href="README.md"><img src="https://img.shields.io/badge/docs-English-blueviolet.svg" alt="English documentation"></a>
</p>

---

يبسّط إقلاع أنظمة التشغيل القياسية من خلال توفير:

- **واجهة موحّدة للإقلاع والكتابة** (Android Fastboot)، مستقلة عن
  التعديلات الخاصة بكل جهاز في سلسلة الإقلاع (مثل واجهة الفلاش المملوكة
  على أجهزة Samsung)
- **كشف تلقائي للجهاز والعناقيد البرمجية** (لوحات العرض، البطاريات، ...)
- **خيارات إقلاع إضافية** (الإقلاع من نظام ملفات، أو من بطاقة SD)
- **ميزات إضافية للبرمجيات الثابتة** (إقلاع أنوية CPU الثانوية عندما تفتقر
  البرمجيات الثابتة إلى دعم [PSCI])
- **حلول بديلة لمشاكل خاصة بأجهزة معينة** (USB على بعض أجهزة Samsung،
  المودم على هواتف Motorola، ...)
- **أوامر تصحيح أخطاء** لأغراض البحث والتطوير في العناقيد البرمجية

---

## كيف يعمل

يأتي lk2nd في تكوّنين:

- **lk2nd** — محمّل إقلاع «ثانوي» للأجهزة التي لا يمكن استبدال برمجياتها
  الثابتة الأصلية بسهولة (معظم الهواتف والأجهزة اللوحية). **لا** يستبدل
  محمّل الإقلاع الأصلي. بدلاً من ذلك، يُغلَّف في صورة إقلاع Android
  يحمّلها محمّل الإقلاع الأصلي مثل أي صورة نظام تشغيل أخرى.
  يوضع نظام التشغيل الحقيقي في قسم الإقلاع بإزاحة 512 كيلوبايت،
  أو في نظام ملفات ext2. لا يجب أن يكون Android أو حتى Linux —
  يمكن وضع أي نوع من النوى في صورة إقلاع Android.

- **lk1st** — محمّل إقلاع أساسي للحواسيب أحادية اللوحة (SBCs)
  والمستخدمين المحترفين. هو محمّل الإقلاع «الأول» المسؤول عن
  تحميل نظام التشغيل الرئيسي.

هذا المستودع هو تفرّع (fork) من [محمّل الإقلاع الأصلي مفتوح المصدر من
Qualcomm](https://git.codelinaro.org/clo/la/kernel/lk/-/tree/lk.lnx.1.0.r54-rel)،
وهو نفسه نسخة معدّلة كثيرًا من نظام [Little Kernel] المدمج.

---

## التثبيت

1. حمّل `lk2nd.img` من [Releases](https://github.com/mohammed-abdelghani/lk2nd-msm8226-msm8228/releases).
2. اكتبها باستخدام واجهة الفلاش الأصلية:

   ```sh
   # Fastboot
   fastboot flash boot lk2nd.img

   # Samsung (Heimdall)
   heimdall flash --BOOT lk2nd.img

   # EDL
   edl w boot lk2nd.img
   ```

إذا ظهر الخطأ `fastboot: error: Couldn't parse partition size '0x'`، جرّب
أحد الحلول البديلة التالية:

```sh
fastboot flash:raw boot lk2nd.img
# أو
fastboot boot lk2nd.img && fastboot flash lk2nd lk2nd.img
```

---

## الاستخدام

بعد التثبيت، يوفّر lk2nd بروتوكول Android fastboot القياسي لكتابة
أنظمة التشغيل وإقلاعها.

- اضغط **Volume Down** أثناء الإقلاع للدخول إلى وضع Fastboot.
- اضغط **Volume Up** أثناء الإقلاع للدخول إلى وضع Recovery.

> [!TIP]
> إذا كان محمّل الإقلاع الأصلي يستخدم نفس مجموعات الأزرار، انتظر لحظة
> قبل الضغط على أزرار الصوت — عادةً حتى تضيء الشاشة أو يهتز الجهاز —
> حتى يتجاهلها محمّل الإقلاع الأصلي.

لتحديث lk2nd نفسه عبر واجهة fastboot الخاصة به:

```sh
fastboot flash lk2nd lk2nd.img
```

> [!IMPORTANT]
> `fastboot flash boot boot.img` يكتب صورة الإقلاع بإزاحة 512 كيلوبايت
> داخل قسم الإقلاع، بحيث لا تستبدل كتابة نظام تشغيل lk2nd نفسه أبدًا.

جميع أوامر fastboot الأخرى تعمل بشكل طبيعي.

### استكشاف الأخطاء وإصلاحها

إذا ظهر الجهاز في fastboot، يمكنك استرجاع ملف سجل lk2nd:

```sh
fastboot oem log && fastboot get_staged output.txt
```

استخدم `/dev/stdout` بدلًا من `output.txt` لطباعة السجل مباشرة.

---

## البناء من المصدر

المتطلبات وخيارات البناء موثّقة في
[building.md](Documentation/building.ar.md). باختصار:

```sh
make TOOLCHAIN_PREFIX=arm-none-eabi- lk2nd-msmXXXX
```

تُكتب الصورة الناتجة في `build-lk2nd-msmXXXX/lk2nd.img`.

راجع [targets.md](Documentation/targets.ar.md) لقائمة أهداف البناء المدعومة.

## الوثائق

| المستند | المحتوى |
| --- | --- |
| [boot.md](Documentation/boot.ar.md) | طرق الإقلاع (صورة الإقلاع، fastboot، extlinux.conf) ومعاملات سطر الأوامر |
| [building.md](Documentation/building.ar.md) | متطلبات البناء وأعلامه |
| [devices.md](Documentation/devices.ar.md) | الأجهزة المدعومة |
| [targets.md](Documentation/targets.ar.md) | أهداف البناء |
| [dt-bindings.md](Documentation/dt-bindings.ar.md) | صيغة شجرة الأجهزة |
| [fastboot.md](Documentation/fastboot.ar.md) | أوامر fastboot في lk2nd |

