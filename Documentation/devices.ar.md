# الأجهزة المدعومة
[English](devices.md)


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

## تغطية DTB الدنيا

تتطلب أحدث الأجهزة المدعومة من lk2nd (خصوصًا أجهزة SDM429/SDM439/SDM632)
تثبيت صورة قسم DTBO مخصصة ودنيا. وإلا،
سيحاول محمّل الإقلاع تصحيح أشجار الأجهزة الخاصة بـ lk2nd بالتراكب (overlay) الأصلي
الذي يشير إلى جميع العقد التي لم تعد موجودة، ويرفض المتابعة.


## نقل (Porting) أجهزة جديدة

إذا كنت تفكر في إضافة دعم لجهاز جديد، ستحتاج إلى إضافة
إدخال dts جديد. يمكنك الاطلاع على [targets.md](targets.ar.md) لاختيار
الهدف الصحيح لرقاقة نظامك واستشارة [dt-bindings.md](dt-bindings.ar.md)
لصيغة dts الخاصة بـ lk2nd والتعريفات (bindings).
