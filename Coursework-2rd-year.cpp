#include <iostream>
#include <unordered_map>
#include <windows.h>

using namespace std;

class TrieNode { // вершина (узел) дерева
public:
    unordered_map<char, TrieNode*> children; // хеш-таблица для хранения символа, соответствующего данной вершине и ссылок на все дочерние вершины
    bool isEndOfWord;  // флаг, указывающий, является ли данная вершина концом слова

    TrieNode() {  // конструктор класса TrieNode (узла дерева)
        isEndOfWord = false;
    }
};

class Trie { // само дерево
public:

    // создание нового объекта класса TrieNode с помощью оператора "new" и присваиваивание ссылки на этот экземпляр переменной "root", которая является корневым узлом дерева
    Trie() { // конструктор класса Trie
        root = new TrieNode(); // инициализация корневого узла дерева
    }

    // ф-я вставки нового слова в дерево (Асимптотическая сложность в худшем случае O(m*n), m - длина искомого слова, n - кол-во узлов в дереве)
    void insert(string word) { // аргументом ф-ии является строка "word", которую необходимо добавить в дерево
        TrieNode* current = root; // создание указателя на корневой узел дерева
        for (int i = 0; i < word.length(); i++) { // цикл по каждому символу в слове
            char c = word[i]; // фиксируем текущий символ в слове
            if (current->children.find(c) == current->children.end()) { // проверка, существует ли узел дерева (есть ли потомок у "current"), соответствующий текущему символу (метод find (определяет элемент, соответствующий указанному ключу) используется для поиска дочерних узлов у текущего узла. Если find возвращает итератор, указывающий на конец children, то это означает, что текущий символ отсутствует в дереве)
                current->children[c] = new TrieNode(); // если узел не найден, создается новый узел с помощью оператора "new", и его указатель сохраняется в карте дочерних узлов children текущего узла.
            }
            current = current->children[c]; // в противном случае, если символ "c" найден, мы переходим к соответствующему дочернему узлу current
        }
        current->isEndOfWord = true; // после того, как все символы слова были добавлены в дерево, устанавливается флаг "isEndOfWord" в текущем узле. Это означает, что слово закончилось в этом узле и он (узел) является конечной точкой для данного слова в дереве.
    }

    // ф-я поиска заданного слова в дереве (Асимптотическая сложность в худшем случае O(m*n), m - длина искомого слова, n - кол-во узлов в дереве)
    bool search(string word) { // аргументов ф-ии является строка "word", которую необходимо найти в дереве
        TrieNode* current = root; // инициализация указателя "current" на корень дерева
        for (int i = 0; i < word.length(); i++) { // цикл по каждому символу в заданном слове
            char c = word[i]; // фиксируем текущий символ в слове
            if (current->children.find(c) == current->children.end()) { // проверка, существует ли узел дерева (есть ли потомок у "current"), соответствующий текущему символу (метод find (определяет элемент, соответствующий указанному ключу) используется для поиска дочерних узлов у текущего узла. Если find возвращает итератор, указывающий на конец children, то это означает, что текущий символ отсутствует в дереве)
                return false; // если узел не найден, возвращаем "false"
            }
            current = current->children[c]; // в противном случае, если символ "c" найден, мы переходим к соответствующему дочернему узлу "current"
        }
        return current->isEndOfWord; // после того как ф-я обходит все символы word, указатель current указывает на последний символ в word. Если узел, на который указывает current, имеет свойство isEndOfWord, то это означает, что word найден в дереве, и функция возвращает true. В противном случае функция возвращает false.
    }

    // ф-я удаления заданного слова в дереве (Асимптотическая сложность в худшем случае O(m*n), m - длина удаляемого слова, n - кол-во узлов в дереве)
    void remove(string word) {
        TrieNode* current = root;
        vector<TrieNode*> path; // вектор, содержащий путь от корневого узла до последнего узла буквы слова

        for (char c : word) { // цикл по каждой букве в слове
            if (current->children.find(c) == current->children.end()) {
                cout << "Дерево не содержит слово " << word << endl;
                return; // слово не существует в дереве
            }
            path.push_back(current);
            current = current->children[c];
        }
        if (!current->isEndOfWord) { // если просмотрели всё слово (дошли до последнего символа в слове) и флаг isEndOfWord равен false, то
            cout << "Дерево не содержит слово " << word << endl;
            return; // слово не существует в дереве
        }

        current->isEndOfWord = false; // устанавливается флаг isEndOfWord последнего узла current в false, чтобы пометить слово как удаленное

        if (!current->children.empty()) { // если слово отмечено как удалённое, но у последнего символа остались потомки, то никакие узлы не удаляем
            cout << "Слово " << word << " удалено" << endl;
            return;
        }
        for (int i = path.size() - 1; i >= 0; i--) { // если у слова, которое отмечено как удалённое, нет потомков, то выполняется обратный цикл по вектору path, начиная с последнего элемента
            TrieNode* node = path[i];
            char c = word[i];
            node->children.erase(c); // удаляется узел, соответствующий текущему символу (узел не является концом слова и не имеет потомков)
            if (node->isEndOfWord || !node->children.empty()) { // если текущий узел является концом другого слова или имеет других потомков, то удаление прекращается, чтобы сохранить другие слова с общим префиксом. В противном случае продолжается удаление.
                break;
            }
        }
        cout << "Слово " << word << " удалено" << endl;
        return;
    }

    // ф-я находит все слова в префиксном дереве, начинающиеся с заданного префикса
    // (выполняет рекурсивный обход префиксного дерева, начиная с заданного узла node, и добавляет все найденные слова в вектор results)
    // Асимптотическая сложность в худшем случае O(n^2), n - кол-во узлов в дереве
    void findAllWords(TrieNode* node, string prefix, vector<string>& results) {
        if (node->isEndOfWord) { // если текущий узел является концом слова, то добавляем слово в вектор результатов results
            results.push_back(prefix);
        }

        for (auto& child : node->children) { // перебираем все возможные дочерние узлы текущего узла
            char c = child.first; // cохраняем символ дочернего узла child в переменную c
            findAllWords(child.second, prefix + c, results); // рекурсивно вызываем функцию findAllWords для следующего дочернего узла с обновленным префиксом
        }
    }

    // ф-я проходит по каждому узлу дерева, соответствующего очередному символу префикса, останавливаясь на узле, с которого нужно искать слова в дереве, которые начинаются с заданного префикса
    // Асимптотическая сложность функции startsWith в худшем случае будет O(m * n + n^2), где m - длина префикса, а n - количество узлов в Trie
    vector<string> startsWith(string prefix) {
        TrieNode* current = root;
        for (int i = 0; i < prefix.length(); i++) {
            char c = prefix[i];
            if (current->children.find(c) == current->children.end()) {
                return {};
            }
            current = current->children[c];
        }

        // Если мы успешно прошли по всем символам префикса, находим все слова, начинающееся с данного префикса с помощью ф-ии findAllWords
        vector<string> results; // Вектор, для хранения в нём результата
        findAllWords(current, prefix, results); // передаём указатель на текущий узел, текущий префикс и пустой вектор для ответа

        return results;
    }

private:
    TrieNode* root; // объявление указателя на корень дерева
};

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    Trie trie;

    trie.remove("apple"); // такого слова нет
    trie.insert("banana");
    trie.remove("ban");   // такого слова нет

    trie.insert("app");
    cout << trie.search("apple") << endl;             // Return false
    cout << trie.search("app") << endl;               // Return true
    vector<string> results = trie.startsWith("app"); // Return "app"
    for (string word : results) {
        cout << word << " ";
    }
    cout << endl;

    trie.insert("apple");

    cout << trie.search("app") << endl;               // Returns true
    cout << trie.search("apple") << endl;             // Return true
    cout << trie.search("application") << endl;       // Return false
    results = trie.startsWith("app");                 // Returns "app", "apple"
    for (string word : results) {
        cout << word << " ";
    }
    cout << endl;

    trie.insert("bag");
    trie.remove("app");
    trie.insert("applaud");
    trie.remove("apple");

    trie.insert("application");
    results = trie.startsWith("ap");              // Returns "application", "applaud"
    for (string word : results) {
        cout << word << " ";
    }
    cout << endl;

    trie.insert("blue");
    trie.remove("bag");

    results = trie.startsWith("b");                 // Returns  "banana", "blue"
    for (string word : results) {
        cout << word << " ";
    }
    cout << endl;

    return 0;
}

/*
Решение задачи с комментариями

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct TrieNode { // структура узла префиксного дерева
    unordered_map<char, TrieNode*> children;
    bool is_word; // флаг, указывающий, является ли текущий узел концом слова
    int prefix_count; // количество слов, которые имеют префикс, заканчивающийся в текущей вершине
    TrieNode() : is_word(false), prefix_count(0) {}
};

// Ф-я принимает вектор строк words и подсчитывает общее количество нажатий клавиш, необходимых для ввода этих слов, учитывая возможное автодополнение
int count_keystrokes(const vector<string>& words) {
    int keystrokes = 0; // общее кол-во нажатий на клавиши
    TrieNode* root = new TrieNode(); // создание корневого узла дерева

    for (const string& word : words) { // цикл по каждому слову
        TrieNode* node = root; // указатель на текущий узел
        int pos = 0; // текущая позиция в слове
        int startPos = 0;
        int endPos = -1;

        // найдем первую вершину в боре, которая соответствует переходу по текущей букве строки, и существует ровно один префикс из ранее рассмотренных слов, оканчивающийся в этой вершине
        while (pos < word.length() && node->children.count(word[pos]) > 0) { // пока не достигнут конец слова и пока текущий узел имеет дочерний узел, соответствующий следующей букве в слове
            node = node->children[word[pos]]; // переход к след узлу, соответствующему следующей букве в слове
            pos++; // переход к след букве в слове

            if (node->prefix_count == 1) {
                startPos = pos; // посмотрели первые pos букв строки
                break;
            }
        }

        // пойдем дальше по строке с позиции pos. Будем увеличивать pos до тех пор, пока количество префиксов, оканчивающихся в соответствующей вершине бора, равно одному.
        while (pos < word.length() && node->children.count(word[pos]) > 0) { // то же условие цикла
            node = node->children[word[pos]];
            pos++;

            if (node->prefix_count == 1 && node->is_word) { // если вершина бора, в которой мы остановились, является листом
                endPos = pos; // фиксируем текущую позицию
                break;
            }
        }

        if (endPos == -1) { // Если префикс не найден, добавляем длину всего слова (так как польностью набираем его сами)
            keystrokes += word.length();
        }
        else {    // Если найден, то буквы с startPos до endPos, мы можем набрать с помощью одного нажатия на клавишу автодополнения,
            keystrokes += startPos + 1 + word.length() - endPos; // и нужно прибавить к ответу startPos (так как эти буквы мы введём без автодополнения), прибавить один (это нажатие на кнопку автодополнения)
        }         // и прибавить разность между длиной строки и endPos (так как иногда останется ненабранный суффикс, который нужно набрать руками после использования автодополнения)


        // Добавляем слово в бор
        if (word.length() != endPos) { // если произошло автодополнение до полного слова, то это делать не нужно, т.к слово целиком есть в дереве
            node = root;
            for (int i = 0; i < word.length(); i++) {
                if (node->children.count(word[i]) == 0) {
                    node->children[word[i]] = new TrieNode();
                }
                node = node->children[word[i]];
                node->prefix_count++; // Увеличивается счетчик prefix_count текущего узла, чтобы отразить, что еще одно слово проходит через данный префикс
            }
            node->is_word = true;
        }
    }
    return keystrokes;
}

// Ф-я, которая отделяет от текста слова (записываются в вектор строк) и считает кол-во разделителей (знаков препинания)
int separateText(const string& text) {
    vector<string> words;
    string word;
    int separators = 0;
    for (char c : text) {
        if (c == ' ' || c == '\n' || c == '.' || c == ',' || c == '?' || c == '!' || c == '\'' || c == '-') {
            separators++;
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    int keystrokes = count_keystrokes(words);
    keystrokes += separators;
    return keystrokes;
}

int main() {
    string line, text;
    while (getline(cin, line)) { // построковый ввод текста
        text += line + '\n';
    }
    if (text.empty()) {
        cout << 0 << endl;
        return 0;
    }
    int keystrokes = separateText(text);
    cout << keystrokes << endl;
    return 0;
}
*/

/*
Тесты к задаче

snow affects sports such as skiing, snowboarding, and snowmachine travel.
snowboarding is a recreational activity and olympic and paralympic sport.


thun-thun-thunder, thunder, thunder
thunder, thun-, thunder
thun-thun-thunder, thunder
thunder, feel the thunder
lightning then the thunder
thunder, feel the thunder
lightning then the thunder
thunder, thunder
*/