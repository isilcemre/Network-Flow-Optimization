#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

// Kenar (Yol) yapısı. Kalıntı grafikte akış ve maliyet takibi için kullanılır.
typedef struct Edge {
    int to;          // Hedef şehir
    int capacity;    // Maksimum taşıma kapasitesi (truc)
    int flow;
    int cost;        // the distance
    int reverse_edge; // Residual graph için geri kenarın index’i (çok önemli)
} Edge;

// Komşuluk listesi düğümü. Her şehrin komşularını dinamik dizi olarak tutar.
typedef struct AdjList {
    Edge *edges;    // Kenarların dinamik dizisi
    int count;      // Mevcut kenar sayısı
    int capacity;   // Dizi kapasitesi
} AdjList;

// Grafik yapısı
typedef struct {
    int n_cities;   // Şehir sayısı
    int n_roads;    // Yol sayısı
    AdjList *adj;   // Komşuluk listelerinin dinamik dizisi
} Graph;

// Global değişkenler
Graph g;
int SOURCE = 1; // s
int SINK;       // t



// Yeni bir grafik başlatır
void init_graph(int n, int m) {
    g.n_cities = n;
    g.n_roads = m;
    SINK = n; // Bitiş şehri n'dir

    // Şehirler 1'den n'ye numaralandığı için n+1 boyutunda bellek tahsis et
    g.adj = (AdjList*)calloc(n + 1, sizeof(AdjList));
    if (g.adj == NULL) {
        perror("Graph bellek tahsisi hatasi");
        exit(EXIT_FAILURE);
    }

    // Her şehrin komşuluk listesini başlat
    for (int i = 1; i <= n; i++) {
        g.adj[i].capacity = 4; // Başlangıç kapasitesi (küçük bir değer)
        g.adj[i].edges = (Edge*)malloc(g.adj[i].capacity * sizeof(Edge));
        g.adj[i].count = 0;
        if (g.adj[i].edges == NULL) {
            perror("AdjList bellek tahsisi hatasi");
            exit(EXIT_FAILURE);
        }
    }
}

// Komşuluk listesine kenar ekler (dinamik diziye ekleme)
void add_edge_to_list(int u, Edge e) {
    AdjList *list = &g.adj[u];
    // Eğer kapasite dolduysa, kapasiteyi iki katına çıkar
    if (list->count == list->capacity) {
        list->capacity *= 2;
        list->edges = (Edge*)realloc(list->edges, list->capacity * sizeof(Edge));
        if (list->edges == NULL) {
            perror("Realloc hatasi");
            exit(EXIT_FAILURE);
        }
    }
    list->edges[list->count++] = e;
}

// İki yönlü kenarları (ileri ve geri) oluşturur
void add_edge(int u, int v, int cap, int cost) {
    // 1. İleri Kenar (u -> v)
    // cap, bu kenarın orijinal kapasitesini tutar.
    Edge forward = {v, cap, 0, cost, g.adj[v].count};
    add_edge_to_list(u, forward);

    // 2. Geri Kenar (v -> u)
    // Geri kenarın kapasitesi başlangıçta 0'dır (kalıntı akış için), maliyeti tersi (-cost)
    Edge backward = {u, 0, 0, -cost, g.adj[u].count - 1}; // forward kenarının indeksini kaydet
    add_edge_to_list(v, backward);
}

// Grafiğin belleğini serbest bırakır
void free_graph() {
    for (int i = 1; i <= g.n_cities; i++) {
        free(g.adj[i].edges);
    }
    free(g.adj);
}

// Girdi okuma fonksiyonu
void read_input() {
    int n, m;
    // Şehir sayısı ve yol sayısını oku
    if (scanf("%d %d", &n, &m) != 2) return;
    init_graph(n, m);

    // Yol bilgilerini oku: v1, v2, capacity, length
    for (int i = 0; i < m; i++) {
        int v1, v2, cap, cost;
        if (scanf("%d %d %d %d", &v1, &v2, &cap, &cost) != 4) {
            fprintf(stderr, "Hata: Yol bilgileri okunamadi.\n");
            exit(EXIT_FAILURE);
        }
        // İki yönlü (ileri ve geri) kenarı grafiğe ekle
        add_edge(v1, v2, cap, cost);
    }
}

// --- Algoritma 1: Maksimum Akış (Edmonds-Karp) ---

/**
 * Edmonds-Karp Max-Flow Algoritması
 * BFS kullanarak kalıntı grafikte artırılabilir yol bulur.
 * */
int max_flow_edmonds_karp() {

    int total_flow = 0;
    int n = g.n_cities;

    // parent_edge: yolu yeniden oluşturmak için (parent_edge[v] = u'daki kenar indeksi)
    int *parent_edge = (int*)malloc((n + 1) * sizeof(int));
    // parent_node: yolu yeniden oluşturmak için (parent_node[v] = u)
    int *parent_node = (int*)malloc((n + 1) * sizeof(int));
    // path_flow: o anki yoldan geçebilecek maksimum akış
    int *path_flow = (int*)malloc((n + 1) * sizeof(int));

    if (!parent_edge || !parent_node || !path_flow) {
        perror("Max Flow bellek tahsisi hatasi");
        exit(EXIT_FAILURE);
    }

    // Maksimum akış döngüsü: Artırılabilir yol olduğu sürece devam et
    while (1) {
        // BFS başlat: Kalıntı grafikte SOURCE'dan SINK'e yol bul
        for (int i = 1; i <= n; i++) {
            parent_edge[i] = -1;
            parent_node[i] = -1;
            path_flow[i] = 0;
        }

        // Kuyruk (Queue) için basit bir dizi kullan
        int *queue = (int*)malloc((n + 1) * sizeof(int));
        int head = 0, tail = 0;

        queue[tail++] = SOURCE;
        parent_node[SOURCE] = SOURCE; // Başlangıcı işaretle
        path_flow[SOURCE] = INT_MAX; // Kaynağa sınırsız akış

        int path_found = 0;

        // BFS döngüsü
        while (head < tail) {
            int u = queue[head++];

            if (u == SINK) {
                path_found = 1;
                break;
            }

            // u'nun komşularını gez
            for (int i = 0; i < g.adj[u].count; i++) {
                Edge *e = &g.adj[u].edges[i];
                int v = e->to;
                // Eğer v ziyaret edilmediyse VE kalıntı kapasite varsa
                if (parent_node[v] == -1 && e->capacity - e->flow > 0) {
                    parent_node[v] = u;
                    parent_edge[v] = i;
                    // Bu yoldan geçebilecek maksimum akışı güncelle
                    path_flow[v] = (e->capacity - e->flow < path_flow[u]) ? e->capacity - e->flow : path_flow[u];
                    queue[tail++] = v;
                }
            }
        }
        free(queue);

        // Eğer SINK'e ulaşılamadıysa (artırılabilir yol yoksa), döngüyü bitir
        if (!path_found) {
            break;
        }

        // Bulunan yoldan akışı artır
        int flow_increase = path_flow[SINK];
        total_flow += flow_increase;

        // Yolu geriye doğru takip et ve akışları güncelle
        int curr = SINK;
        while (curr != SOURCE) {
            int prev = parent_node[curr];
            int edge_index = parent_edge[curr];

            // İleri kenarın akışını artır
            Edge *fwd_edge = &g.adj[prev].edges[edge_index];
            fwd_edge->flow += flow_increase;

            // Geri kenarın (kalıntı kenar) akışını azalt (ters akış kapasitesini artır)
            int rev_index = fwd_edge->reverse_edge;
            Edge *rev_edge = &g.adj[curr].edges[rev_index];
            rev_edge->capacity += flow_increase; // Geri kenarın kapasitesi artırılır

            curr = prev;
        }
    }


    printf("1) Maksimum Kamyon Sayisi: %d\n", total_flow);

    // Belleği serbest bırak
    free(parent_edge);
    free(parent_node);
    free(path_flow);

    return total_flow;
}

// --- Algoritma 2: Minimum Maliyetli Maksimum Akış (Min-Cost Max-Flow) ---

// Min Cost Max Flow'da Bellman-Ford'un yerini alan potansiyelli Dijkstra için
// Min-Cost Max-Flow'u daha verimli hale getirir.
long long min_cost_max_flow(int max_flow) {

    int n = g.n_cities;
    long long total_cost = 0;
    int current_flow = 0;

    // Potansiyel (pi) dizisi: Dijkstra'nın çalışmasını hızlandırır (negatif döngü riskini önler)
    long long *pi = (long long*)calloc(n + 1, sizeof(long long));
    if (pi == NULL) {
        perror("Min Cost bellek tahsisi hatasi (pi)");
        exit(EXIT_FAILURE);
    }

    // Max-Flow algoritması grafiğin akış ve kapasite değerlerini değiştirdiği için,
    // Min-Cost hesaplamasına başlamadan önce grafiği orijinal durumuna döndürmek gerekir.
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j < g.adj[i].count; j++) {
            Edge *e = &g.adj[i].edges[j];
            e->flow = 0; // Akışı sıfırla

            // Eğer kenar geri kenarsa (cost < 0), kapasitesini de başlangıçtaki 0'a sıfırla.
            // İleri kenarın kapasitesi (cost >= 0) değişmemelidir.
            if (e->cost < 0) {
                e->capacity = 0;
            }
        }
    }

    // Ana döngü: Maksimum akış elde edilene kadar en kısa (en ucuz) yolu bul ve akışı artır
    while (current_flow < max_flow) {
        // dist: Başlangıçtan i'ye olan en kısa maliyet (Dijkstra)
        long long *dist = (long long*)malloc((n + 1) * sizeof(long long));
        // parent_edge / parent_node: Yolu yeniden oluşturmak için
        int *parent_edge = (int*)malloc((n + 1) * sizeof(int));
        int *parent_node = (int*)malloc((n + 1) * sizeof(int));

        if (!dist || !parent_edge || !parent_node) {
            perror("Min Cost bellek tahsisi hatasi (dist/parent)");
            free(dist); free(parent_edge); free(parent_node);
            return -1;
        }

        // Başlangıç durumu
        for (int i = 1; i <= n; i++) {
            dist[i] = LLONG_MAX; // Sonsuz
            parent_edge[i] = -1;
            parent_node[i] = -1;
        }
        dist[SOURCE] = 0;

        // --- Dijkstra Algoritması (Potansiyelli) ---

        // visited dizisi: Düğümün kalıcı olarak etiketlenip etiketlenmediğini gösterir
        int *visited = (int*)calloc(n + 1, sizeof(int));
        if (visited == NULL) {
            perror("Min Cost bellek tahsisi hatasi (visited)");
            exit(EXIT_FAILURE);
        }

        // Basit Dijkstra: Her seferinde minimum mesafeli düğümü doğrusal arama ile bul
        for (int k = 0; k < n; k++) {
            long long min_dist = LLONG_MAX;
            int u = -1;

            // Ziyaret edilmemiş minimum dist'e sahip düğümü bul
            for (int i = 1; i <= n; i++) {
                if (!visited[i] && dist[i] < min_dist) {
                    min_dist = dist[i];
                    u = i;
                }
            }

            if (u == -1) break; // Yol yok veya kalan tüm düğümler sonsuz
            visited[u] = 1;

            // u'nun komşularını gez
            for (int i = 0; i < g.adj[u].count; i++) {
                Edge *e = &g.adj[u].edges[i];
                int v = e->to;

                // Kalıntı kapasite: Akış artırılabilir mi?
                if (e->capacity - e->flow > 0 && !visited[v]) {

                    // İndirgenmiş Maliyet (Reduced Cost) hesaplaması:
                    // rc(u,v) = cost(u,v) + pi(u) - pi(v)
                    // Dijkstra'nın her zaman pozitif maliyetlerle çalışmasını sağlar.
                    long long reduced_cost = (long long)e->cost + pi[u] - pi[v];

                    // Relaksasyon: Daha kısa (ucuz) bir yol bulunduysa
                    if (dist[v] > dist[u] + reduced_cost) {
                        dist[v] = dist[u] + reduced_cost;
                        parent_node[v] = u;
                        parent_edge[v] = i;
                    }
                }
            }
        }

        // --- Dijkstra Sonu ---

        // SINK'e bir yol bulunamadıysa (veya maliyet sonsuzsa), max_flow'a ulaşıldı
        if (dist[SINK] == LLONG_MAX) {
            // Akışın Max-Flow'a ulaşmadan kesilmesi, Min-Cost hesaplamasının bittiğini gösterir.
            break;
        }

        // Potansiyelleri güncelle (pi(v) = pi(v) + dist(v))
        for (int i = 1; i <= n; i++) {
            if (dist[i] != LLONG_MAX) {
                pi[i] += dist[i];
            }
        }

        // Yoldan geçebilecek akışı bul (Max-Flow'a kadar kalan akışla sınırlı)
        int push_flow = max_flow - current_flow;

        // Bulunan yoldan geçebilecek maksimum akışı (izin verilen kapasiteyi) bul
        int curr = SINK;
        while (curr != SOURCE) {
            int prev = parent_node[curr];
            int edge_index = parent_edge[curr];

            // Kalıntı kapasite: capacity - flow
            int residual_capacity = g.adj[prev].edges[edge_index].capacity - g.adj[prev].edges[edge_index].flow;

            if (residual_capacity < push_flow) {
                push_flow = residual_capacity;
            }
            curr = prev;
        }

        // Akışı güncelle ve maliyeti hesapla
        current_flow += push_flow;

        // Maliyet hesaplaması: Artırılan akış * Yolun gerçek maliyeti
        long long path_real_cost = 0;
        curr = SINK;
        while (curr != SOURCE) {
            int prev = parent_node[curr];
            int edge_index = parent_edge[curr];

            // Gerçek maliyeti topla (cost'u kullan, reduced_cost değil)
            path_real_cost += g.adj[prev].edges[edge_index].cost;

            // İleri kenarın akışını artır
            Edge *fwd_edge = &g.adj[prev].edges[edge_index];
            fwd_edge->flow += push_flow;

            // Geri kenarın (kalıntı kenar) kapasitesini artır
            int rev_index = fwd_edge->reverse_edge;
            Edge *rev_edge = &g.adj[curr].edges[rev_index];
            // Geri kenarın kapasitesini (yani akışını) artır (akış ters yöne dönebilir)
            rev_edge->capacity += push_flow;

            curr = prev;
        }

        total_cost += push_flow * path_real_cost;

        // Belleği serbest bırak
        free(dist);
        free(parent_edge);
        free(parent_node);
        free(visited);
    }

    free(pi);

    printf("2) En Kisa Mesafe Icin Toplam Maliyet: %lld\n", total_cost);

    return total_cost;
}


int main() {

    // Girdi verilerini oku ve grafiği oluştur
    read_input();

    printf("(%d  cities, %d roads)\n", g.n_cities, g.n_roads);

    // (Max-Flow)
    int max_trucks = max_flow_edmonds_karp();


    // (Min-Cost Max-Flow)
    if (max_trucks > 0) {
        min_cost_max_flow(max_trucks);
    } else {
        printf("2) En Kisa Mesafe Icin Toplam Maliyet: 0\n");
        printf("   Maksimum akis sifir oldugu icin maliyet hesaplanmadi.\n");
    }

    free_graph();

    return 0;
}