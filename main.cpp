#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <stack>
#include <ctime>

// Еще одна интересная идея реализации - это использовать алгоритм Уоршела, если мы получим матрицу из единиц, то граф сильно связный.
// Подумать про алгоритм с проверкой на эйлеровость и поиска эйлерова цикла вместе


using namespace std;
const int MaxV = 26;

char Ch(int c) {    // Возвращает c-ый символ алфавита
    return c + 'a';
}

class GR {
    int n, m;                           // Кол-во вершин и ребер
    int in_deg[MaxV] = {0};             // Массив количества входящих ребер 
    int out_deg[MaxV] = {0};            // Массив количества выходящих ребер
    vector < list < int >> LIST;        // Задаем вектор, элементы которого - списки

    //--приватные функции конструкторы и операторы для отслеживания их случайного вызова-- 
	GR(const GR &);
	GR(GR &&);

	GR operator=(const GR &) const = delete;
	GR operator=(GR &&) const = delete;	
    public:
        GR(int, int, int &, int &);
        // Проверка на существование эйлерова цикла
        bool kosaraju();  // Проверка на сильно связность
        bool in_out_deg();  // Проверка на то, что для каждой вершины сумма входяших равна сумме выходящих ребер
        void Hierholzer_printCurciut(); // Поиск эйлерова цикла через алгоритм Хирхольцера
        ~GR() =
            default; // указывает компилятору самостоятельно генерировать деструктор
};

GR::GR(int MaxV, int menu, int &v, int &e): n(0), m(0) {
    string s;
    int G[MaxV][MaxV];                  // Матрица смежности
    int start,
        k1,
        k2,
        counter;
    for (int i = 0; i < MaxV; ++i)
        for (int j = 0; j < MaxV; ++j) G[i][j] = 0;

    switch(menu)
    {
        case 1:
            // Ввод данных в матрицу смежности
            cout << "Enter the number of vertices in the graph\n";
            do {
                cin >> n;
                if(n < 0 || n > 26) cout << "The number of vertices range from 0 to 26";
            } while(n < 0 || n > 26);

            counter = 0;
            cout << "\nEnter adjacency sets (strings of letters a through z)\n";
            do {
                cout << "v[" << Ch(counter) << "] = ";
                // cout << "v[" << n << "]= ";
                cin >> s;
                for (auto i: s)                     // Эквивалентно for(auto s = string.begin(); s != string.end(); ++s)
                    if (isalpha(i)) {               // Игнорируем символы не принадлежащие алфавиту
                        int j = (tolower(i) - 'a'); // tolower возвращает строчный эквивалент символа
                        G[counter][j] = 1;          // Ориентированный граф
                        (in_deg[j])++;              // Увеличиваем количество входящих ребер в j на 1
                        (out_deg[counter])++;
                    }
                counter++;
            } while (isalpha(s[0]) && counter < n);      // Пока первый символ введенной строки не 0 и n не превышает MaxV
        break;
        case 2:
            // Генерация случайной матрицы смежности
            cout << "Enter the number of vertices in the graph\n";
            do {
                cin >> n;
                if(n < 0 || n > 26) cout << "The number of vertices range from 0 to 26";
            } while(n < 0 || n > 26);
            
            for (int i = 0; i < n; ++i)
                for (int j = i; j < n; ++j)
                    if (G[i][j] = rand()%2)
                    {
                        (in_deg[j])++;
                        (out_deg[i])++;
                    }
        break;
        case 3:
            // Генерация эйлерова графа
            // Вместо того, чтобы строить граф по определенным признакам - мы будем строить эйлеров цикл
            cout << "Enter the number of vertices in the graph\n";
            do {
                cin >> n;
                if(n < 0 || n > 26) cout << "The number of vertices range from 0 to 26";
            } while(n < 0 || n > 26);

            counter = 0;
            if(n) {
                start = rand()%(n);
                k1 = start;
                k2 = rand()%(n);
                do
                {
                    if(!G[k1][k2]) {        // Чтобы отслеживать реальное кол-во out/in_deg (нужно для алгоритма Хирхольцера)
                        G[k1][k2] = 1;
                        k1 = k2;
                        (in_deg[k2])++;
                        (out_deg[k1])++;
                        k2 = rand()%(n);
                    }
                    if(counter++ > MaxV*MaxV) k2 = start;  // Как показывает практика, k2 может никогда не стать start, поэтому делаем это вручную
                }
                while(k1 != start);
            }
        break;
        case 4:
            // Заготовленный пример
            n = 5;
            G[0][1] = 1; G[1][2] = 1; G[1][3] = 1; G[2][0] = 1; G[3][4] = 1; G[4][1] = 1;
            for(int i = 0; i < n; i++)
            {
                for(int j = 0; j < n; j++)
                    in_deg[i] += G[i][j];
                out_deg[i] = in_deg[i];
            }
            // {0, 1, 0, 0, 0},
            // {0, 0, 1, 1, 0},
            // {1, 0, 0, 0, 0},
            // {0, 0, 0, 0, 1},
            // {0, 1, 0, 0, 0},
        break;
        default:
            cout << "menu range from 0 - 4!\n";
    }

    // Формирование и вывод списка смежности
    cout << "Graph list:";
    LIST.resize(n); // Изменение размера 
    for (int i = 0; i < n; ++i) {
        int f = 0;
        cout << '\n' << Ch(i) << ": ";
        // cout << '\n' << i << ": ";
        for (int j = 0; j < n; ++j)
            if (G[i][j]) {
                ++f;
                LIST[i].push_back(j);
                cout << Ch(j) << ' ';
                //cout << j << ' ';
            }
        else cout << "- ";
        m += f;
    }
    cout << "\n|V|=" << n << " |E|=";  m ? cout << m / 2 << '\n': cout << m << '\n'; // На случай, если m = 1
    v = n; e = m;
}

bool GR::in_out_deg() {
    for(int i = 0; i < n; i++)
        if(out_deg[i] != in_deg[i]) return false;

    return true;
}

bool GR::kosaraju() {
    int i = 0;  // Вершина, степень которой больше нуля
    auto size = n;
    std::vector<bool> vis(size);           // По умолчанию все значения инициализирует как ложь
    vector < list < int >> t(size);         // Транспонированный граф

    // Раз разрешено пользоваться всей библиотекой STL, то решил изучить для себя лямбда выражения и обертки

    // std::function - полиморфная обертка, способная хранить, копировать и ссылаться на любой обьект. В том числе и лямбда-выражение.
    // Полиморфный - способный обрабатывать данные разных типов
    std::function<void(int)> visit; // Создаем обертку visit, которая ничего не возвращает и принимает в качестве параметра int

    // В глубину
    // Храним лямбда-выражение в обертке visit
    // Лямбда-выражение - краткая форма записи анонимных функторов (функтор - обьект, который можно использовать как функцию)
    visit = [&](int u) {    // [&] - Позволяет нам захватить переменную по ссылке. (int u) - используемая переменная в лямбда-выражении
        if (!vis[u]) {
            vis[u] = true;
            for (auto v : LIST[u]) {
                visit(v);
                t[v].push_back(u); // Меняем направление, т.е. строим транспонированный граф
            }
        }
    };
    while(!out_deg[i]) i++; // Доходим до вершины с ненулевой степенью
    visit(i);

    for (int j = 0; j < size; ++j)
        if(!vis[j] && out_deg[j]) return false;    // Если найдется не посещенная вершина, степень которой больше 0, то граф не сильно связный, а значит не содержит эйлерова цикла

    // Аналогично проходимся по транспонированному графу
    std::function<void(int)> assign;
    assign = [&](int u) {
        if (vis[u]) {
            vis[u] = false;
            for (auto v : t[u]) {
                assign(v);
            }
        }
    };
    assign(i);

    for (i = 0; i < size; ++i)
        if(vis[i] && out_deg[i]) return false;
    
    return true;
}

// Алгоритм Хирхольцера
// Выбираем любую начальную вершину v и следуем по ребрам от этой вершины пока не вернемся к v. 
// Невозможно застрять ни в одной вершине, кроме v, потому что четная степень всех вершин гарантирует, 
// что, когда мы входим в другую вершину w, должно быть неиспользуемое ребро, выходящее из w.
// Сформированный таким образом путь является циклом, но он может не охватывать все ребра исходного графа.
// (1) Теперь добавляем в вектор вершины, у которых не осталось инцидентных ребер.
// (2) После этого возвращаемся к вершине u, у которой есть инцидентные ребра и следуем по ребрам из этой вершины пока не вернемся к u
// Повторяем шаги (1) и (2). Полученный вектор - будет упорядоченный эйлеров цикл, но почему?

// Формального доказательства я не нашел как и в принципе любого другого, но из здравого смысла можно додумать следующее:
// Первый получаемый цикл, если он не эйлеров - не проходит по всем ребрам, но если бы мы нашли цикл, который дополнил наш существующий,
// то мы бы приблизились к эйлерову, что собственно и делает наш алгоритм. И логичным утверждением будет, что дополняющий цикл,
// должен иметь пересечение с существующим через вершину, которая еще имеет неиспользуемые инцидентные ребра
void GR::Hierholzer_printCurciut() {
    // if (!LIST.size())
    // {
    //     cout << "graph empty";
    //     return; // Пустой граф
    // }

    int i = 0,  // Вершина с ненулевой степенью
        next_v, 
        curr_v; // Текущая вершина
    // Текущий путь
    stack<int> curr_path; 
    // Эйлеров цикл
    vector<int> circuit; 

    while(!out_deg[i]) i++;
    curr_path.push(i); 
    curr_v = i;

    while (!curr_path.empty()) 
    {
        // Если есть инцидентные ребра, то
        if (out_deg[curr_v]) 
        { 
            // Добавляем в путь очередную вершину
            curr_path.push(curr_v); 
            // Достаем следующую вершину смежную с curr_v
            next_v = LIST[curr_v].back(); 
            // и удаляем ребро ведущее к next_v
            out_deg[curr_v]--;
            LIST[curr_v].pop_back();
            // Переходим к следующей вершине
            curr_v = next_v; 
        } 

        // Если нет, то
        else
        {
            // Добавляем вершину в эйлеров цикл
            circuit.push_back(curr_v); 

            // Достаем последнюю вершину в пути и раз мы ее рассматриваем на следующем шаге, то удалим ее из стека
            curr_v = curr_path.top(); 
            curr_path.pop(); 
        } 
    }

    // Вывод
    cout << "----------------------------------------\nEuler cycle: ";
    for (int i=circuit.size()-1; i>=0; i--) 
    { 
        cout << Ch(circuit[i]);
        // cout << circuit[i];
        if (i) 
            cout<<" -> "; 
    }
    cout << "\n----------------------------------------\n";
}

int main() {
    
    srand(time(nullptr));
    int menu,
        v, e;
    do
    {
        cout << "1 - Enter the graph\n";
        cout << "2 - Generate random graph\n";
        cout << "3 - Generate Euler graph\n";
        cout << "4 - Use ready example\n";
        cout << "0 - Exit\n";
        cin >> menu;
        cout << "----------------------------------------\n";
        if(menu < 5 && menu > 0) {
            GR graph(MaxV, menu, v, e);
            if(!v) cout << "----------------------------------------\nThe graph has zero vertices\n----------------------------------------\n";
            else if(!e) cout << "----------------------------------------\nThe graph has zero edges\n----------------------------------------\n";
            else 
                if(graph.in_out_deg() && graph.kosaraju())
                    graph.Hierholzer_printCurciut();
                else cout << "----------------------------------------\nThe graph is not Euler\n----------------------------------------\n";
        }
        else if(menu) cout << "Menu range from 0 to 4!\n";
    } while(menu);

    return 0;
}