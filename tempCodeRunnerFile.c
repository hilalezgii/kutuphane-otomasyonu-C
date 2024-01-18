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
        if (sscanf(satir, "%d %s %s %s %d %s", &kitap.kitapId, kitap.kitapAdi, kitap.kitapYazar, kitap.kategori, &kitap.sayfaSayisi, &kitap.kitapDurum) != 6)
        {
            printf("Hatali satir formati: %s\n", satir);
            continue;
        }

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
        printf("Dosya acilamadi!");
        return;
    }

    struct Kitap kitap;
    int bulunanId;
    char geciciDurumStr[10]; // "true" veya "false" değeri için geçici bir karakter dizisi

    int kitapBulundu = 0; // Güncellenecek kitap bulundu mu kontrolü

    while (fscanf(dosya, "%d %s %s %s %d %s", &bulunanId, kitap.kitapAdi, kitap.kitapYazar, kitap.kategori, &kitap.sayfaSayisi, geciciDurumStr) != EOF)
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

        if (bulunanId == arananId)
        {
            // Kitabi bulduk, güncelleme işlemi yapilacak
            printf("Yeni kitap bilgilerini girin:\n");

            printf("\nKitabin Adini giriniz: ");
            scanf(" %[^\n]", kitap.kitapAdi);

            printf("\nKitabin Yazarini giriniz: ");
            scanf(" %[^\n]", kitap.kitapYazar);

            printf("\nKitabin Sayfa Sayisini giriniz: ");
            scanf("%d", &kitap.sayfaSayisi);

            printf("\nKitabin Kategorisini giriniz: ");
            scanf(" %[^\n]", kitap.kategori);

            kitap.kitapDurum = true;
            kitapBulundu = 1; // Kitap bulundu olarak işaretle

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
        remove("temp.txt"); // Silinmeyen temp dosyasını siliyoruz
    }
    else
    {
        // Eski dosyayi sil ve temp dosyasini adini değiştir
        remove("kitap_kayitlari.txt");
        rename("temp.txt", "kitap_kayitlari.txt");
        printf("Kitap basariyla guncellendi.\n");
    }
}

int kitapSil(int silinecekId)
{
      FILE *dosya = fopen("kitap_kayitlari.txt", "r");
    FILE *tempDosya = fopen("temp.txt", "w");

    if (dosya == NULL || tempDosya == NULL)
    {
        printf("Dosya acilamadi!");
        return 0;
    }

    struct Kitap kitap;
    int bulunanId;
    int kitapBulundu = 0;

    while (fscanf(dosya, "%d %s %s %s %d %s\n", &bulunanId, kitap.kitapAdi, kitap.kitapYazar, kitap.kategori, kitap.sayfaSayisi, &kitap.kitapDurum) != EOF)
    {
        if (bulunanId == silinecekId)
        {
            if (!kitap.kitapDurum)
            {
                kitapBulundu = 1;
                continue;
            }
            else
            {
                printf("kitap odunc alindigi icin silme islemi yapilamaz.\n");
                fclose(dosya);
                fclose(tempDosya);
                return 0;
            }
        }

        fprintf(tempDosya, "%d %s %s %s %d %s\n", bulunanId, kitap.kitapAdi, kitap.kitapYazar, kitap.kategori, kitap.sayfaSayisi, kitap.kitapDurum);
    }

    fclose(dosya);
    fclose(tempDosya);

    if (!kitapBulundu)
    {
        printf("Silinecek kitap bulunamadi.\n");
        remove("temp.txt");
        return 0;
    }
    else
    {
        remove("kitap_kayitlari.txt");
        rename("temp.txt", "kitap_kayitlari.txt");
        printf("kitap basariyla silindi.\n");
        return 1;
    }
}
