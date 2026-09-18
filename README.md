# DeneyYapKart

Arduino uyumlu bir geliştirme kartı ile **20x4 I2C LCD ekran** üzerinde dinamik veri gösterimi için hazırlanmış örnek proje.

Proje; LCD ekranı satır ve sütunlara bölerek farklı veri düzenlerini göstermek, ekrandaki değerleri belirli aralıklarla güncellemek ve birden fazla ekran görünümü arasında otomatik geçiş yapmak amacıyla geliştirilmiştir.

## Özellikler

- 20x4 I2C LCD desteği
- LCD ve Serial Monitor'e eş zamanlı veri yazdırma
- Satır bazında farklı sütun sayıları kullanabilme
- Sütun genişliklerini ayrı ayrı belirleyebilme
- Uzun metinleri otomatik kırpma
- Kısa metinleri boşluk ile tamamlayarak ekranı temiz tutma
- `millis()` kullanarak bloklamayan zaman kontrolü
- Belirli aralıklarla veri güncelleme
- Farklı ekran düzenleri arasında otomatik geçiş

## Proje Yapısı

```text
DeneyYapKart/
├── sketch_jul20a/
│   └── sketch_jul20a.ino
├── sketch_jul21a/
│   └── sketch_jul21a.ino
├── sketch_jul21c/
│   └── sketch_jul21c.ino
└── README.md
```

### Sketch'ler

| Dosya | Açıklama |
|---|---|
| `sketch_jul20a` | LCD bağlantısı, temel metin gösterimi ve özel karakter denemesi |
| `sketch_jul21a` | 20x4 LCD üzerinde sabit robot durum bilgilerinin gösterildiği deneme |
| `sketch_jul21c` | **Ana proje.** Dinamik veri, sütunlu ekran düzeni, periyodik güncelleme ve ekran geçişleri |

## Gereksinimler

### Donanım

- Arduino uyumlu geliştirme kartı
- 20x4 LCD ekran
- I2C LCD dönüştürücü/modül
- Jumper kablolar

### Kütüphaneler

Projede aşağıdaki kütüphaneler kullanılmaktadır:

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
```

- `Wire` Arduino ortamıyla birlikte gelir.
- `LiquidCrystal_I2C` kütüphanesinin Arduino IDE üzerinden ayrıca kurulması gerekebilir.

Arduino IDE'de:

**Sketch > Include Library > Manage Libraries** menüsünü açıp `LiquidCrystal I2C` araması yaparak uygun kütüphaneyi yükleyebilirsiniz.

## LCD Bağlantısı

I2C LCD modülündeki temel pinler:

| LCD | Geliştirme Kartı |
|---|---|
| VCC | 5V veya kartın/modülün desteklediği besleme |
| GND | GND |
| SDA | Kartın SDA pini |
| SCL | Kartın SCL pini |

> SDA ve SCL pinleri kullanılan geliştirme kartına göre değişebilir. Kartınızın pin şemasını kontrol edin.

Projede varsayılan LCD adresi `0x27` olarak tanımlanmıştır:

```cpp
LiquidCrystal_I2C lcd(0x27, 20, 4);
```

Ekranınız çalışmıyorsa I2C adresinin farklı olup olmadığını kontrol edin. Bazı modüllerde `0x3F` gibi farklı adresler kullanılabilir.

## Ana Projenin Çalışma Mantığı

Ana uygulama:

```text
sketch_jul21c/sketch_jul21c.ino
```

LCD ekranın boyutları ve desteklenen maksimum sütun sayısı şu sabitlerle tanımlanır:

```cpp
#define EKRAN_GENISLIGI 20
#define EKRAN_SATIR_SAYISI 4
#define MAKS_SUTUN_SAYISI 4
```

### Ekran güncelleme süreleri

```cpp
const unsigned long GUNCELLEME_ARALIGI = 1000;
const unsigned long EKRAN_DEGISIM_ARALIGI = 5000;
```

Buna göre:

- Ekrandaki örnek değerler her **1 saniyede** güncellenir.
- Ekran düzeni her **5 saniyede** değiştirilir.

Zaman kontrolü `delay()` yerine `millis()` ile yapıldığı için program bekleme sırasında bloke olmaz.

## Ekran Düzenleri

### Örnek 1

Dört LCD satırının tamamını kullanır:

```text
ROBOT HAZIR
HIZ:70    BAT:%100
X:120  Y:85   A:45
M1:1 M2:1 M3:0 M4:1
```

Satırlardaki sütun dağılımları:

```cpp
{1, 2, 3, 4}
```

Sütun genişlikleri ise ekranın toplam 20 karakterlik alanını dolduracak şekilde tanımlanmıştır:

```cpp
{20, 0, 0, 0}
{10, 10, 0, 0}
{7, 7, 6, 0}
{5, 5, 5, 5}
```

### Örnek 2

İki satırlık alternatif bir görünüm kullanır:

```text
HIZ:70    BAT:%100
ILERI  DUR   GERI
```

Bu yapı, aynı LCD üzerinde farklı arayüz ekranlarının kolayca oluşturulabileceğini göstermek için eklenmiştir.

## Temel Fonksiyonlar

### `hucreHazirla()`

Bir hücre içindeki metni verilen sütun genişliğine göre düzenler.

- Metin uzunsa kırpar.
- Metin kısaysa boşluk ekler.

Bu sayede her hücre sabit genişlikte tutulur.

### `ekranYazdir()`

Verileri tanımlanan satır/sütun yapısına göre:

1. LCD ekrana,
2. Serial Monitor'e

yazdırır.

Ayrıca kullanılmayan ekran alanlarını boşluklarla temizleyerek önceki verilerin LCD üzerinde kalmasını engeller.

### `birinciOrnegiGoster()` ve `ikinciOrnegiGoster()`

Farklı ekran tasarımlarının verilerini, sütun sayılarını ve sütun genişliklerini tanımlar.

Yeni ekranlar oluşturmak için aynı yapıda yeni fonksiyonlar eklenebilir.

## Kullanım

1. Depoyu bilgisayarınıza indirin veya klonlayın.
2. Arduino IDE'yi açın.
3. Ana proje için aşağıdaki dosyayı açın:

```text
sketch_jul21c/sketch_jul21c.ino
```

4. `LiquidCrystal_I2C` kütüphanesinin kurulu olduğundan emin olun.
5. Geliştirme kartınızı ve doğru portu seçin.
6. Kodu karta yükleyin.
7. LCD ekranı gözlemleyin.
8. İsterseniz Serial Monitor'ü **115200 baud** hızında açarak aynı verileri seri port üzerinden de takip edin.

## Özelleştirme

### Güncelleme süresini değiştirmek

Örneğin verilerin 500 ms'de bir güncellenmesi için:

```cpp
const unsigned long GUNCELLEME_ARALIGI = 500;
```

### Ekran geçiş süresini değiştirmek

Örneğin ekranların 10 saniyede bir değişmesi için:

```cpp
const unsigned long EKRAN_DEGISIM_ARALIGI = 10000;
```

### Yeni veri eklemek

`veriler` dizisindeki metinler sensör, motor, batarya veya diğer sistemlerden alınan gerçek değerlerle değiştirilebilir:

```cpp
String veriler[EKRAN_SATIR_SAYISI][MAKS_SUTUN_SAYISI] = {
  {"SISTEM HAZIR", "", "", ""},
  {"HIZ:" + String(hiz), "BAT:%" + String(batarya), "", ""},
  {"X:120", "Y:85", "A:45", ""},
  {"M1:1", "M2:1", "M3:0", "M4:1"}
};
```

### Yeni ekran oluşturmak

Yeni bir görünüm oluştururken üç temel bilgi tanımlanmalıdır:

1. Gösterilecek veriler
2. Her satırdaki sütun sayısı
3. Her sütunun karakter genişliği

Bir satırdaki sütun genişliklerinin toplamının **20 karakteri aşmaması** önerilir.

## Geliştirme Fikirleri

Proje aşağıdaki özelliklerle genişletilebilir:

- Gerçek sensör verilerinin LCD'de gösterilmesi
- Motor durumlarının gerçek zamanlı takibi
- Batarya gerilimi ve yüzde hesabı
- Butonlarla ekranlar arasında manuel geçiş
- Menü sistemi
- Hata ve uyarı ekranları
- I2C sensörlerle entegrasyon
- Seri porttan gelen verilerin LCD'ye aktarılması

## Not

`sketch_jul20a` ve `sketch_jul21a` klasörlerindeki kodlar geliştirme ve test amacıyla hazırlanmıştır. Güncel ana uygulama `sketch_jul21c` klasöründedir.
