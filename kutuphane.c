#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// Kitap struct'i
struct Kitap
{
    char kitapAdi[50];
    int kitapId;
    char kitapYazar[50];
    char kategori[25];
    int sayfaSayisi;
    bool kitapDurum;
};

// Kullanici struct'i
struct Kullanici
{
    char kullaniciId;
    char kullaniciAdi[50];
    char kullaniciSoyad[50];
    bool kullaniciDurum;
    char kullaniciDurumStr[5];
};

// Odunc struct'i
struct Odunc
{
    int kullaniciId;
    int kitapId;
    char baslangicTarihi[20];
    char bitisTarihi[20];
};
// random sayi olusturma

int randomUIDD()
{
    srand((unsigned int)time(NULL));
    return rand();
}
void kitapEkle(struct Kitap *kitap)
{
    printf("\nKitabin Adini giriniz: ");
    scanf(" %[^\n]", kitap->kitapAdi); // %[^ karakterleri Enter tuşuna kadar olan tüm karakterleri alir.

    printf("\nKitabin Yazarini giriniz: ");
    scanf(" %[^\n]", kitap->kitapYazar);

    printf("\nKitabin Sayfa Sayisini giriniz: ");
    scanf("%d", &kitap->sayfaSayisi);

    printf("\nKitabin Kategorisini giriniz: ");
    scanf(" %[^\n]", kitap->kategori);

    kitap->kitapDurum = false;

    kitap->kitapId = randomUIDD();

    // Dosyaya yazma islemi
    FILE *dosya = fopen("kitap_kayitlari.txt", "a");

    if (dosya == NULL)
    {
        printf("Dosya acilamadi!");
        return;
    }

    // Kitap bilgilerini dosyaya yazma
    fprintf(dosya, "%d %s %s %s %d %s\n", kitap->kitapId, kitap->kitapAdi, kitap->kitapYazar, kitap->kategori, kitap->sayfaSayisi, kitap->kitapDurum ? "true" : "false");

    // Dosyayi kapat
    fclose(dosya);

    printf("\nTebrikler kitap ekleme islemi basarili\n");
}
void kitapListele()
{
    FILE *dosya = fopen("kitap_kayitlari.txt", "r");
    char satir[4096];

    if (dosya == NULL)
    {
        printf("Dosya acilamadi!\n");
        return;
    }

    struct Kitap kitap;

    printf("%-15s%-15s%-15s%-25s%-15s%-15s\n", "Kitap Id", "Kitap Adi", "Kitap Yazar", "Kategori", "Sayfa Sayisi", "Kitap Durum");

    while (fgets(satir, sizeof(satir), dosya) != NULL)
    {
        int okunan;
        char durumChar; // Geçici olarak durumu char olarak okuyacak değişken

        if (sscanf(satir, "%d %s %s %s %d %c", &kitap.kitapId, kitap.kitapAdi, kitap.kitapYazar, kitap.kategori, &kitap.sayfaSayisi, &durumChar) != 6)
        {
            printf("Hatali satir formati: %s\n", satir);
            continue;
        }

        // Char türünden bool türüne dönüştürme
        kitap.kitapDurum = (durumChar == 'T' || durumChar == 't');

        // Kitap bilgilerini ekrana bas
        printf("%-15d%-15s%-15s%-25s%-15d%-15s\n", kitap.kitapId, kitap.kitapAdi, kitap.kitapYazar, kitap.kategori, kitap.sayfaSayisi, kitap.kitapDurum ? "true" : "false");
    }

    fclose(dosya);
}
void kitapGuncelle()
{
    int arananId;
    printf("Guncellenecek kitabin ID'sini girin: ");
    scanf("%d", &arananId);

    FILE *dosya = fopen("kitap_kayitlari.txt", "r");
    FILE *tempDosya = fopen("temp.txt", "w");

    if (dosya == NULL || tempDosya == NULL)
    {
        printf("Dosya acilamadi!\n");
        return;
    }

    struct Kitap kitap;
    int bulunanId;
    char geciciDurumStr[10];

    int kitapBulundu = 0;

    while (fscanf(dosya, "%d %s %s %s %d %s", &bulunanId, kitap.kitapAdi, kitap.kitapYazar, kitap.kategori, &kitap.sayfaSayisi, geciciDurumStr) == 6)
    {
        // Gecici durum stringini bool değere çevirme
        kitap.kitapDurum = (geciciDurumStr[0] == 't' || geciciDurumStr[0] == 'T');

        if (bulunanId == arananId)
        {
            // Kitabi bulduk, güncelleme işlemi yapilacak
            printf("Yeni kitap bilgilerini girin:\n");

            printf("\nKitabin Adini giriniz : ");
            scanf(" %[^\n]", kitap.kitapAdi);

            printf("\nKitabin Yazarini giriniz : ");
            scanf(" %[^\n]", kitap.kitapYazar);

            printf("\nKitabin Sayfa Sayisini giriniz : ");
            scanf("%d", &kitap.sayfaSayisi);

            printf("\nKitabin Kategorisini giriniz : ");
            scanf(" %[^\n]", kitap.kategori);

            printf("\nKitap Durumunu giriniz (true/false): ");
            scanf(" %s", geciciDurumStr);

            if (strcmp(geciciDurumStr, "true") == 0)
            {
                kitap.kitapDurum = true;
            }
            else if (strcmp(geciciDurumStr, "false") == 0)
            {
                kitap.kitapDurum = false;
            }
            else
            {
                printf("Hatali giris: Gecersiz kitap durumu\n");
                fclose(dosya);
                fclose(tempDosya);
                return;
            }

            kitapBulundu = 1;

            // Güncellenmiş kitap bilgilerini temp dosyasina yaz
            fprintf(tempDosya, "%d %s %s %s %d %s\n", arananId, kitap.kitapAdi, kitap.kitapYazar, kitap.kategori, kitap.sayfaSayisi, kitap.kitapDurum ? "true" : "false");
        }
        else
        {
            // Bulunmayan kitaplari temp dosyasina yaz
            fprintf(tempDosya, "%d %s %s %s %d %s\n", bulunanId, kitap.kitapAdi, kitap.kitapYazar, kitap.kategori, kitap.sayfaSayisi, kitap.kitapDurum ? "true" : "false");
        }
    }

    fclose(dosya);
    fclose(tempDosya);

    if (!kitapBulundu)
    {
        printf("Guncellenecek kitap bulunamadi.\n");
        remove("temp.txt");
    }
    else
    {
        // Eski dosyayi sil ve temp dosyasini adini değiştir
        remove("kitap_kayitlari.txt");
        rename("temp.txt", "kitap_kayitlari.txt");
        printf("Kitap basariyla guncellendi.\n");
    }
}

void kitapSil()
{
    FILE *dosya = fopen("kitap_kayitlari.txt", "r");
    FILE *tempDosya = fopen("temp.txt", "w");

    if (dosya == NULL || tempDosya == NULL)
    {
        printf("Dosya acilamadi!");
        return;
    }

    struct Kitap kitap;
    int silinecekId;
    char geciciDurumStr[10];

    printf("Silinecek kitabin ID'sini girin: ");
    scanf("%d", &silinecekId);

    int kitapSilindi = 0; // Silinen kitap kontrolu

    while (fscanf(dosya, "%d %s %s %s %d %s", &kitap.kitapId, kitap.kitapAdi, kitap.kitapYazar, kitap.kategori, &kitap.sayfaSayisi, geciciDurumStr) != EOF)
    {
        // Gecici durum stringini bool değere çevirme
        if (strcmp(geciciDurumStr, "true") == 0)
        {
            kitap.kitapDurum = true;
        }
        else if (strcmp(geciciDurumStr, "false") == 0)
        {
            kitap.kitapDurum = false;
        }
        else
        {
            printf("Hatali dosya formati: Gecersiz kitap durumu\n");
            fclose(dosya);
            fclose(tempDosya);
            return;
        }

        if (kitap.kitapId == silinecekId)
        {
            if (kitap.kitapDurum == false)
            {
                // Kitap ödünç alınmamışsa silme işlemi gerçekleştirilir
                printf("Kitap silindi:\n");
                kitapSilindi = 1;
            }
            else
            {
                // Kitap ödünç alındığı için silme işlemi yapılamaz
                printf("Kitap ödünç alındığı için silme işlemi yapılamaz.\n");
            }
        }
        else
        {
            // Silinmeyecek kitapları temp dosyasına yaz
            fprintf(tempDosya, "%d %s %s %s %d %s\n", kitap.kitapId, kitap.kitapAdi, kitap.kitapYazar, kitap.kategori, kitap.sayfaSayisi, kitap.kitapDurum ? "true" : "false");
        }
    }

    fclose(dosya);
    fclose(tempDosya);

    if (!kitapSilindi)
    {
        printf("Silinecek kitap bulunamadi.\n");
        remove("temp.txt"); // Silinmeyen temp dosyasını siliyoruz
    }
    else
    {
        // Eski dosyayi sil ve temp dosyasini adini değiştir
        remove("kitap_kayitlari.txt");
        rename("temp.txt", "kitap_kayitlari.txt");
    }
}
void search()
{
    int secim;
    printf("Arama yapmak istediginiz kitap adini girin: ");
    scanf("%d", &secim);

    FILE *dosya = fopen("kitap_kayitlari.txt", "r");

    if (dosya == NULL)
    {
        printf("Dosya acilamadi!");
        return;
    }

    struct Kitap kitap;
    int arananId;
    char arananKelime[50];
    bool bulunan = false;

    printf("Aranan kitap adini girin: ");
    scanf("%s", arananKelime);

    char line[4095 + 1];
    while (fgets(line, sizeof line, dosya))
        if (strstr(line, arananKelime))
            printf("\nArama sonucu: ");
    printf("%s", line);
    bulunan = true;

    fclose(dosya);

    if (!bulunan)
    {
        printf("Aranan kriterde kitap bulunamadi.\n");
    }
}
void kullaniciEkle(struct Kullanici *kullanici)
{
    printf("\nKullanici Adini giriniz: ");
    scanf("%s", kullanici->kullaniciAdi);

    printf("\nKullanici soyadi giriniz: ");
    scanf("%s", kullanici->kullaniciSoyad);

    kullanici->kullaniciDurum = false;
    // kullaniciId ekleme islemi
    kullanici->kullaniciId = randomUIDD(); // randomUIDD fonksiyonu kullanicinin tanimli olduğunu varsayalim

    FILE *dosya = fopen("kullanici_kayitlari.txt", "a");

    if (dosya == NULL)
    {
        printf("Dosya acilamadi!");
        return;
    }

    fprintf(dosya, "%d %s %s %s\n", kullanici->kullaniciId, kullanici->kullaniciAdi, kullanici->kullaniciSoyad, kullanici->kullaniciDurum ? "true" : "false");
    fclose(dosya);

    printf("\nTebrikler kullanici ekleme islemi basarili\n");
}
void kullaniciListele()
{
    FILE *dosya = fopen("kullanici_kayitlari.txt", "r");
    char satir[100];

    if (dosya == NULL)
    {
        printf("Dosya acilamadi!");
        return;
    }

    struct Kullanici kullanici;

    printf("%-15s%-20s%-20s%-10s\n", "Kullanici Id", "Adi", "Soyadi", "Durumu");
    printf("--------------------------------------------------------\n");

    while (fscanf(dosya, "%d %s %s %s", &kullanici.kullaniciId, kullanici.kullaniciAdi, kullanici.kullaniciSoyad, kullanici.kullaniciDurumStr) == 4)
    {
        kullanici.kullaniciDurum = (strcmp(kullanici.kullaniciDurumStr, "true") == 0);

        printf("%-15d%-20s%-20s%-10s\n", kullanici.kullaniciId, kullanici.kullaniciAdi, kullanici.kullaniciSoyad, kullanici.kullaniciDurum ? "true" : "false");
    }

    fclose(dosya);
}

void kullaniciGuncelle()
{
    int arananId;
    printf("Guncellenecek kullanicinin ID'sini girin: ");
    scanf("%d", &arananId);

    FILE *dosya = fopen("kullanici_kayitlari.txt", "r");
    FILE *tempDosya = fopen("temp.txt", "w");

    if (dosya == NULL || tempDosya == NULL)
    {
        printf("Dosya acilamadi!");
        return;
    }

    struct Kullanici kullanici;
    int bulunanId;
    char geciciDurumStr[10]; // "true" veya "false" değeri için geçici bir karakter dizisi

    while (fscanf(dosya, "%d %99s %99s %s", &bulunanId, kullanici.kullaniciAdi, kullanici.kullaniciSoyad, geciciDurumStr) == 4)
    {
        // Gecici durum stringini bool değere çevirme
        if (strcmp(geciciDurumStr, "true") == 0)
        {
            kullanici.kullaniciDurum = true;
        }
        else if (strcmp(geciciDurumStr, "false") == 0)
        {
            kullanici.kullaniciDurum = false;
        }
        else
        {
            printf("Hatali dosya formati: Gecersiz kullanici durumu\n");
            fclose(dosya);
            fclose(tempDosya);
            return;
        }

        if (bulunanId == arananId)
        {
            // Kullaniciyi bulduk, güncelleme işlemi yapilacak
            printf("Yeni kullanici bilgilerini girin:\n");

            printf("\nKullanici adini giriniz: ");
            scanf(" %99s", kullanici.kullaniciAdi);

            printf("\nKullanici soyadini giriniz: ");
            scanf(" %99s", kullanici.kullaniciSoyad);

            kullanici.kullaniciDurum = true;

            // Güncellenmiş kullanici bilgilerini temp dosyasina yaz
            fprintf(tempDosya, "%d %s %s %s\n", arananId, kullanici.kullaniciAdi, kullanici.kullaniciSoyad, kullanici.kullaniciDurum ? "true" : "false");
        }
        else
        {
            // Bulunmayan kullanicilari temp dosyasina yaz
            fprintf(tempDosya, "%d %s %s %s\n", bulunanId, kullanici.kullaniciAdi, kullanici.kullaniciSoyad, kullanici.kullaniciDurum ? "true" : "false");
        }
    }

    fclose(dosya);
    fclose(tempDosya);

    // Eski dosyayi sil ve temp dosyasini adini değiştir
    remove("kullanici_kayitlari.txt");
    rename("temp.txt", "kullanici_kayitlari.txt");

    printf("Kullanici basariyla guncellendi.\n");
}

void kullaniciSilme()
{
    FILE *dosya = fopen("kullanici_kayitlari.txt", "r");
    FILE *tempDosya = fopen("temp.txt", "w");

    if (dosya == NULL || tempDosya == NULL)
    {
        printf("Dosya acilamadi!");
        return;
    }

    struct Kullanici kullanici;
    int silinecekId;
    char geciciDurumStr[10];

    printf("Silinecek kullanicinin ID'sini girin: ");
    scanf("%d", &silinecekId);

    int kullaniciSilindi = 0; // Silinen kullanici kontrolü

    while (fscanf(dosya, "%d %s %s %s", &kullanici.kullaniciId, kullanici.kullaniciAdi, kullanici.kullaniciSoyad, geciciDurumStr) != EOF)
    {
        if (strcmp(geciciDurumStr, "true") == 0)
        {
            kullanici.kullaniciDurum = true;
        }
        else if (strcmp(geciciDurumStr, "false") == 0)
        {
            kullanici.kullaniciDurum = false;
        }
        else
        {
            printf("Hatali dosya formati: Gecersiz kullanici durumu\n");
            fclose(dosya);
            fclose(tempDosya);
            return;
        }

        if (kullanici.kullaniciId == silinecekId)
        {
            printf("Kullanici silindi:\n");
            kullaniciSilindi = 1;
        }
        else
        {
            printf("Kullanici bulunamadi.\n");

            // Silinmeyecek kullanicilari temp dosyasina yaz
            fprintf(tempDosya, "%d %s %s %s\n", kullanici.kullaniciId, kullanici.kullaniciAdi, kullanici.kullaniciSoyad, kullanici.kullaniciDurum ? "true" : "false");
        }
    }

    fclose(dosya);
    fclose(tempDosya);

    if (!kullaniciSilindi)
    {
        printf("Silinecek kullanici bulunamadi.\n");
        remove("temp.txt"); // Silinmeyen temp dosyasini siliyoruz
    }
    else
    {
        // Eski dosyayi sil ve temp dosyasini adini değiştir
        remove("kullanici_kayitlari.txt");
        rename("temp.txt", "kullanici_kayitlari.txt");
    }
}
void kullanicisearch()
{
    int secim;
    printf("Arama yapmak istediginiz kullanici adini girin: ");
    scanf("%d", &secim);

    FILE *dosya = fopen("kullanici_kayitlari.txt", "r");

    if (dosya == NULL)
    {
        printf("Dosya acilamadi!");
        return;
    }

    struct Kullanici kullanici;
    int arananId;
    char arananKelime[50];
    bool bulunan = false;

    scanf("%s", arananKelime);

    char line[4095 + 1];
    while (fgets(line, sizeof line, dosya))
        if (strstr(line, arananKelime))
            printf("\nArama sonucu: ");
    printf("%s", line);
    bulunan = true;

    fclose(dosya);

    if (!bulunan)
    {
        printf("\nAranan kriterde kullanici bulunamadi.\n");
    }
}

void oduncAlmaIslemleri()
{
    int kullaniciId, kitapId;
    char baslangicTarihi[20];

    printf("\nHangi kullanici odunc alacak (kullaniciId): ");
    scanf("%d", &kullaniciId);

    // Kullanici kontrolü 

    printf("\nHangi kitabi odunc almak istiyor (kitapId): ");
    scanf("%d", &kitapId);

    // Kitap kontrolü 

    // Ödünç alma işlemi için başlangiç tarihi 
    printf("\nBaslangic tarihini girin (gg.aa.yyyy): ");
    scanf("%s", baslangicTarihi);

    // Ödünçler dosyasina ekleme yapar
    FILE *oduncDosya = fopen("odunc_kayitlari.txt", "a");
    if (oduncDosya == NULL)
    {
        printf("Dosya acilamadi!");
        return;
    }

    
    fprintf(oduncDosya, "%d %d %s %s\n", kullaniciId, kitapId, baslangicTarihi, "NULL");

    
    fclose(oduncDosya);

    printf("\nOdunc alma islemi basarili.\n");
}

void kitapGeriGetirme()
{
    int kullaniciId, kitapId;
    char bitisTarihi[20];

    printf("\nHangi kullanici kitabi geri getiriyor (kullaniciId): ");
    scanf("%d", &kullaniciId);

    printf("\nHangi kitabi geri getiriyor (kitapId): ");
    scanf("%d", &kitapId);

    // Ödünçler dosyasinda ilgili kaydin bulunp bitiş tarihi al
    FILE *oduncDosya = fopen("odunc_kayitlari.txt", "r");
    FILE *tempDosya = fopen("temp.txt", "w");

    if (oduncDosya == NULL || tempDosya == NULL)
    {
        printf("Dosya acilamadi!");
        return;
    }

    struct Odunc odunc;
    bool bulundu = false;

    while (fscanf(oduncDosya, "%d %d %s %s", &odunc.kullaniciId, &odunc.kitapId, odunc.baslangicTarihi, odunc.bitisTarihi) != EOF)
    {
        if (odunc.kullaniciId == kullaniciId && odunc.kitapId == kitapId && strcmp(odunc.bitisTarihi, "NULL") == 0)
        {
            // İlgili kayit bulundu ve bitiş tarihi aliniyor.
            printf("\nBitis tarihini girin (gg.aa.yyyy): ");
            scanf("%s", bitisTarihi);

            fprintf(tempDosya, "%d %d %s %s\n", odunc.kullaniciId, odunc.kitapId, odunc.baslangicTarihi, bitisTarihi);

            bulundu = true;
            printf("\nKitap basariyla geri getirildi.\n");
        }
        else
        {
            fprintf(tempDosya, "%d %d %s %s\n", odunc.kullaniciId, odunc.kitapId, odunc.baslangicTarihi, odunc.bitisTarihi);
        }
    }

    fclose(oduncDosya);
    fclose(tempDosya);

    // Eski dosyayi sil ve temp dosyasini adini değiştir.
    remove("odunc_kayitlari.txt");
    rename("temp.txt", "odunc_kayitlari.txt");

    if (!bulundu)
    {
        printf("\nIlgili kayit bulunamadi veya kitap zaten geri getirilmis.\n");
    }
}

void oduncListeleme()
{
    FILE *oduncDosya = fopen("odunc_kayitlari.txt", "r");

    if (oduncDosya == NULL)
    {
        printf("Dosya acilamadi!");
        return;
    }

    struct Odunc odunc;

    printf("\n--- Odunc Alma Kayitlari ---\n");
    printf("%-15s%-15s%-20s%-20s\n", "Kullanici Id", "Kitap Id", "Baslangic Tarihi", "Bitis Tarihi");

    while (fscanf(oduncDosya, "%d %d %s %s", &odunc.kullaniciId, &odunc.kitapId, odunc.baslangicTarihi, odunc.bitisTarihi) != EOF)
    {
        printf("%-15d%-15d%-20s%-20s\n", odunc.kullaniciId, odunc.kitapId, odunc.baslangicTarihi, odunc.bitisTarihi);
    }

    fclose(oduncDosya);
}
// Menu fonksiyonu
void menu()
{
    int secim, altsecim, altsecim2, altsecim3;

    do
    {
        printf("*****MENUM*****");
        printf("\n1-Kitap islemleri");
        printf("\n2-Kullanici islemleri");
        printf("\n3-Odunc alma islemleri");
        printf("\n4-Menuden cik");
        printf("\nYapmak istediginiz islemi girin: ");
        scanf("%d", &secim);

        switch (secim)
        {
        case 1:
        {

            do
            {
                printf("*****Kitap Islemleri Menusu*****");
                printf("\n1-Kitap Ekleme");
                printf("\n2-Kitap Listeleme");
                printf("\n3-Kitap Guncelleme");
                printf("\n4-Kitap Silme");
                printf("\n5-Kitap Arama");
                printf("\n6-Ana menuye don");
                printf("\nYapmak istediginiz islemi girin: ");
                scanf("%d", &altsecim);

                switch (altsecim)
                {
                case 1:
                {
                    struct Kitap yeniKitap;
                    kitapEkle(&yeniKitap);
                }
                break;

                case 2:
                {
                    kitapListele();
                }
                break;
                case 3:
                {
                    kitapListele();
                    kitapGuncelle();
                }
                break;
                case 4:
                {
                    kitapListele(); // Kullaniciya mevcut kitaplari listele
                    kitapSil();
                }
                break;
                case 5:
                {
                    search();
                }
                default:
                    break;
                }

            } while (altsecim != 6);
        }

        break;
        case 2:
        {
            do
            {
                printf("*****Kullanici Islemleri Menusu*****");
                printf("\n1-Kullanici Ekleme");
                printf("\n2-Kullanici Listeleme");
                printf("\n3-Kullanici Guncelleme");
                printf("\n4-Kullanici Silme");
                printf("\n5-Kullanici Arama");
                printf("\n6-Ana Menuye Don");
                printf("\nYapmak istediginiz islemi girin: ");
                scanf("%d", &altsecim2);

                switch (altsecim2)
                {
                case 1:
                {
                    struct Kullanici yeniKullanici;
                    kullaniciEkle(&yeniKullanici);
                }
                break;
                case 2:
                {
                    kullaniciListele();
                }
                break;
                case 3:
                {
                    kullaniciListele();
                    kullaniciGuncelle();
                }
                break;
                case 4:
                {
                    kullaniciListele(); // Kullanicilari listele
                    kullaniciSilme();
                }
                break;
                case 5:
                {
                    kullanicisearch();
                }
                break;
                default:
                    break;
                }

            } while (altsecim2 != 6);
        }
        break;

        case 3:
        {
            do
            {
                printf("*****Odunc Alma Islemleri Menusu*****");
                printf("\n1- Kitap Odunc Alma Islemleri");
                printf("\n2- Kitabi Geri Getirme");
                printf("\n3- Odunc Alinan Kitaplari Listele");
                printf("\n4- Ana Menuye Don");
                printf("\nYapmak istediginiz islemi girin: ");
                scanf("%d", &altsecim3);

                switch (altsecim3)
                {
                case 1:
                {
                    oduncAlmaIslemleri();
                }
                break;

                case 2:
                {
                    kitapGeriGetirme();
                }
                break;

                case 3:
                {
                    oduncListeleme();
                }
                break;

                default:
                    break;
                }

            } while (altsecim3 != 4);
        }
        break;
        default:
            break;
        }

    } while (secim != 4);
}

int main()
{
    menu();
    return 0;
}
