# Network Flow Optimization: Max-Truck & Min-Cost Logistics

Bu proje, şehirler arasındaki ulaşım ağını optimize etmek için geliştirilmiş bir **Ağ Akış (Network Flow)** simülasyonudur. Şehirler arası yolların hem belirli bir kamyon kapasitesi hem de bir mesafe maliyeti olduğu bir senaryoda, maksimum verimliliği ve minimum maliyeti hesaplar.

## Proje Amacı

Proje iki temel lojistik problemi çözer:
1. **Maksimum Akış (Max-Flow):** Kaynak şehirden hedef şehre, yol kapasitelerini aşmadan toplamda en fazla kaç kamyon gönderilebilir? (**Edmonds-Karp** algoritması kullanılarak).
2. **Minimum Maliyetli Maksimum Akış (Min-Cost Max-Flow):** Maksimum sayıda kamyonu, toplam kat edilen mesafeyi (maliyet) minimize ederek nasıl ulaştırabiliriz? (**Successive Shortest Path** ve **Bellman-Ford/SPFA** mantığı ile).

## Teknik Özellikler

* **Algoritmalar:** Edmonds-Karp (BFS tabanlı) ve Min-Cost Max-Flow.
* **Veri Yapısı:** Kalıntı Grafik (Residual Graph) ve Komşuluk Listesi (Adjacency List).
* **Bellek Yönetimi:** Dinamik bellek tahsisi ve ters kenar (reverse edge) takibi ile verimli akış güncellenmesi.

## Dosya Yapısı

* `main.c`: Tüm optimizasyon mantığını ve grafik veri yapılarını içeren ana kod.
* `in1.txt`: Örnek ağ yapısını içeren giriş dosyası.
* `task2.exe`: Projenin derlenmiş çalıştırılabilir dosyası.

## Algoritmalar
Bu yazılım arka planda şu karmaşık işlemleri gerçekleştirir:
* **Edmonds-Karp:** Maksimum akışı (toplam kamyon sayısı) bulur.
* **Successive Shortest Path:** Bu akışı en ucuz yoldan göndermek için maliyet optimizasyonu yapar.
* **Residual Graph:** Akışın geri yönlü takibi için kalıntı grafik yapısını kullanır.
