# Network Flow Optimization: Max-Truck & Min-Cost Logistics

This project is a **Network Flow** simulation developed to optimize the transportation network between cities. In a scenario where intercity roads have both a specific truck capacity and a distance cost, it calculates maximum efficiency and minimum cost.

## Project Purpose

The project solves two fundamental logistics problems:
1. **Maximum Flow (Max-Flow):** What is the maximum number of trucks that can be sent from the source city to the destination city without exceeding road capacities? (Calculated using the **Edmonds-Karp** algorithm).
2. **Minimum Cost Maximum Flow (Min-Cost Max-Flow):** How can we transport the maximum number of trucks while minimizing the total distance traveled (cost)? (Calculated using **Successive Shortest Path** and **Bellman-Ford/SPFA** logic).

## Technical Specifications

* **Algorithms:** Edmonds-Karp (BFS-based) and Min-Cost Max-Flow.
* **Data Structure:** Residual Graph and Adjacency List.
* **Memory Management:** Efficient flow updates with dynamic memory allocation and reverse edge tracking.

## File Structure

* `main.c`: The main code containing all optimization logic and graph data structures.
* `in1.txt`: The input file containing the sample network structure.
* `task2.exe`: The compiled executable file of the project.

## Algorithms
This software performs the following complex operations in the background:
* **Edmonds-Karp:** Finds the maximum flow (total number of trucks).
* **Successive Shortest Path:** Performs cost optimization to send this flow via the cheapest route.
* **Residual Graph:** Uses the residual graph structure for backward tracking of the flow.

  

# Ağ Akış Optimizasyonu: Maksimum Kamyon ve Minimum Maliyet Lojistiği

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
