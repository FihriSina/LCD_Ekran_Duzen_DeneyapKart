#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define EKRAN_GENISLIGI 20
#define EKRAN_SATIR_SAYISI 4
#define MAKS_SUTUN_SAYISI 4

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Otomatik güncelleme değişkenleri
unsigned long sonGuncellemeZamani = 0;
unsigned long sonEkranDegisimZamani = 0;

const unsigned long GUNCELLEME_ARALIGI = 1000;  // 1 saniye
const unsigned long EKRAN_DEGISIM_ARALIGI = 5000; // 5 saniye

byte aktifOrnek = 1;
int hiz = 70;
int batarya = 100;


// ------------------------------------------------------
// Hücre metnini belirtilen genişliğe göre hazırlar.
// Uzun metni keser, kısa metni boşlukla tamamlar.
// ------------------------------------------------------
String hucreHazirla(const String &metin, byte genislik) {
  String sonuc = "";

  for (byte i = 0; i < genislik; i++) {
    if (i < metin.length()) {
      sonuc += metin[i];
    } else {
      sonuc += ' ';
    }
  }

  return sonuc;
}


// ------------------------------------------------------
// Verileri hem Serial Monitor'e hem LCD'ye yazdırır.
// ------------------------------------------------------
void ekranYazdir(
  const String veriDizisi[][MAKS_SUTUN_SAYISI],
  byte satirSayisi,
  const byte sutunSayilari[],
  const byte sutunGenislikleri[][MAKS_SUTUN_SAYISI]
) {
  // LCD en fazla 4 satır gösterebilir.
  if (satirSayisi > EKRAN_SATIR_SAYISI) {
    satirSayisi = EKRAN_SATIR_SAYISI;
  }

  Serial.println("+--------------------+");

  // Daima 4 satır işlenir.
  // Kullanılmayan satırlar boşlukla temizlenir.
  for (byte satir = 0; satir < EKRAN_SATIR_SAYISI; satir++) {
    String satirMetni = "";
    byte kullanilanAlan = 0;

    if (satir < satirSayisi) {
      byte sutunSayisi = sutunSayilari[satir];

      if (sutunSayisi > MAKS_SUTUN_SAYISI) {
        sutunSayisi = MAKS_SUTUN_SAYISI;
      }

      for (byte sutun = 0; sutun < sutunSayisi; sutun++) {
        byte genislik = sutunGenislikleri[satir][sutun];

        // Toplam genişliğin 20 karakteri aşmasını engeller.
        if (kullanilanAlan + genislik > EKRAN_GENISLIGI) {
          genislik = EKRAN_GENISLIGI - kullanilanAlan;
        }

        satirMetni += hucreHazirla(
          veriDizisi[satir][sutun],
          genislik
        );

        kullanilanAlan += genislik;

        if (kullanilanAlan >= EKRAN_GENISLIGI) {
          break;
        }
      }
    }

    // Satırı 20 karaktere tamamlar.
    // Böylece önceki yazılar ekranda kalmaz.
    while (satirMetni.length() < EKRAN_GENISLIGI) {
      satirMetni += ' ';
    }

    // Serial Monitor'e yazdırır.
    Serial.print('|');
    Serial.print(satirMetni);
    Serial.println('|');

    // LCD'ye yazdırır.
    lcd.setCursor(0, satir);
    lcd.print(satirMetni);
  }

  Serial.println("+--------------------+");
}


// -------------------------------------------------------
// Örnek 1:
// 4 satır kullanılır.
// Satırlarda sırasıyla 1, 2, 3 ve 4 sütun bulunur.
// -------------------------------------------------------
void birinciOrnegiGoster() {
  String veriler[EKRAN_SATIR_SAYISI][MAKS_SUTUN_SAYISI] = {
    {"ROBOT HAZIR", "",             "",     ""},
    {"HIZ:" + String(hiz), "BAT:%" + String(batarya), "", ""},
    {"X:120",          "Y:85",       "A:45", ""},
    {"M1:1",           "M2:1",       "M3:0", "M4:1"}
  };

  byte sutunSayilari[EKRAN_SATIR_SAYISI] = {
    1, 2, 3, 4
  };

  byte sutunGenislikleri[EKRAN_SATIR_SAYISI]
                         [MAKS_SUTUN_SAYISI] = {
    {20, 0, 0, 0},   // 1 sütun
    {10, 10, 0, 0},  // 2 sütun
    {7, 7, 6, 0},    // 3 sütun
    {5, 5, 5, 5}     // 4 sütun
  };

  ekranYazdir(
    veriler,
    4,
    sutunSayilari,
    sutunGenislikleri
  );
}


// ------------------------------------------------------
// Örnek 2:
// Yalnızca 2 satır kullanılır.
// Birinci satır 2, ikinci satır 3 sütundur.
// ------------------------------------------------------
void ikinciOrnegiGoster() {
  String veriler[EKRAN_SATIR_SAYISI][MAKS_SUTUN_SAYISI] = {
    {"HIZ:" + String(hiz), "BAT:%" + String(batarya), "", ""},
    {"ILERI",              "DUR",                       "GERI", ""},
    {"", "", "", ""},
    {"", "", "", ""}
  };

  byte sutunSayilari[EKRAN_SATIR_SAYISI] = {
    2, 3, 0, 0
  };

  byte sutunGenislikleri[EKRAN_SATIR_SAYISI]
                         [MAKS_SUTUN_SAYISI] = {
    {10, 10, 0, 0},  // 2 sütun
    {7, 6, 7, 0},    // 3 sütun
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  };

  ekranYazdir(
    veriler,
    2,
    sutunSayilari,
    sutunGenislikleri
  );
}


void setup() {
  Serial.begin(115200);

  Wire.begin();
  lcd.init();
  lcd.backlight();

  // İlk ekranın hemen görüntülenmesini sağlar.
  birinciOrnegiGoster();
}


void loop() {
  unsigned long simdikiZaman = millis();

  // Her 5 saniyede örneği değiştirir.
  if (simdikiZaman - sonEkranDegisimZamani
      >= EKRAN_DEGISIM_ARALIGI) {

    sonEkranDegisimZamani = simdikiZaman;

    if (aktifOrnek == 1) {
      aktifOrnek = 2;
    } else {
      aktifOrnek = 1;
    }
  }

  // Her 1 saniyede ekranı günceller.
  if (simdikiZaman - sonGuncellemeZamani
      >= GUNCELLEME_ARALIGI) {

    sonGuncellemeZamani = simdikiZaman;

    // Güncellemenin görülmesi için örnek değerleri değiştirir.
    hiz++;

    if (hiz > 99) {
      hiz = 70;
    }

    batarya--;

    if (batarya < 0) {
      batarya = 100;
    }

    if (aktifOrnek == 1) {
      birinciOrnegiGoster();
    } else {
      ikinciOrnegiGoster();
    }
  }
}