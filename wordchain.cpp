#include <iostream>
#include <string>
#include <vector>
#include <unordered_set> //lägger till ist för vector
#include <algorithm>
#include <unordered_map>
#include <queue>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::unordered_set; //lägger till
using std::unordered_map;
using std::queue;

//typedef vector<string> Dictionary; //Orden ligger i string ["aula", "gula" osv]
typedef std::unordered_set<string> Dictionary; //Byter till unordered_set = snabbare uppslag

//vector där vi sparar grannarna till ordet vi skickar in
vector<string> get_neighbors(const Dictionary &dict, const string &word) {
    vector<string> neighbors;

    //startar med ordingalordet, ändrar sedan en bokstav i taget
    string candidate = word;

    //går igenom de 4 positioner som finns i ordet (OK ENLIGT LABB) + sparar bokstaven som egentligen står på positionen
    for (int i = 0; i < 4; i++) {
        char original = candidate[i];

        //testar bokstäver från a till z, behöver inte testa bokstav som redan finns
        for (char c = 'a'; c <= 'z'; c++) {
            if (c == original)
                continue;

            //byter bokstäver på pos i
            candidate[i] = c;

            //kikar om det finns i ordlistan isf granne
            if (dict.contains(candidate)) {
                neighbors.push_back(candidate);
            }
        }

        //Byter till orginal bokstav sedan gå vidare nästa pos
        candidate[i] = original;
    }

    //retunerar orden som skiljer sig med en bokstav
    return neighbors;
}

/**
 * Hitta den kortaste ordkedjan från 'first' till 'second' givet de ord som finns i
 * 'dict'. Returvärdet är den ordkedja som hittats, första elementet ska vara 'from' och sista
 * 'to'. Om ingen ordkedja hittas kan en tom vector returneras.
 */
vector<string> find_shortest(const Dictionary &dict, const string &from, const string &to) {
    vector<string> result;

    // Kollar om start & slutordet finns i listan
    if (!dict.contains(from) || !dict.contains(to))
        return result;

    //Kö för BFS
    queue<string> queue;

    //Ord som redan besökts
    unordered_set<string> visited;

    //Sparar parent (som man kommer ifrån)
    unordered_map<string, string> parent;

    //BFS från startordet
    queue.push(from);
    visited.insert(from);

    //Fortsätter sålänge det finns ord kvar att undersöka
    while (!queue.empty()) {
        string current = queue.front();
        queue.pop();

        //slutet ordet hittat sluta sök
        if (current == to)
            break;

        //använder neighbors för att hitta grannarna
        vector<string> neighbors = get_neighbors(dict, current);

        for (string neighbor : neighbors) {
            //kollar om redan besökt
            if (!visited.contains(neighbor)) {
                //markerar om besökt
                visited.insert(neighbor);
                //sparar det föräldern vi kom från
                parent[neighbor] = current;
                //lägger in granne i kön
                queue.push(neighbor);
            }
        }
    }

    //Om slutordet ej hittas -> ingen lösning finns
    if (!visited.contains(to))
        return result;

    //börjar vid sista ordet använder parent för att gå tillbaks
    string current = to;

    while (current != from) {
        result.push_back(current);
        current = parent[current];
    }

    //Lägger till startord
    result.push_back(from);

    //vänder på lösningen
    std::reverse(result.begin(), result.end());

    return result;
}

/**
 * Hitta den längsta kortaste ordkedjan som slutar i 'word' i ordlistan 'dict'. Returvärdet är den
 * ordkedja som hittats. Det sista elementet ska vara 'word'.
 */
vector<string> find_longest(const Dictionary &dict, const string &word) {
    vector<string> result;

    // Kollar om start & slutordet finns i listan
    if (!dict.contains(word))
        return result;

    //Kö för BFS
    queue<string> queue;

    //Ord som redan besökts
    unordered_set<string> visited;

    //Sparar parent (som man kommer ifrån)
    unordered_map<string, string> parent;

    //BFS från startordet
    queue.push(word);
    visited.insert(word);

    //Fortsätter sålänge det finns ord kvar att undersöka
    while (!queue.empty()) {
        string current = queue.front();
        queue.pop();

        //Hitta grannar
        vector<string> neighbors = get_neighbors(dict, current);

        for (string neighbor : neighbors) {
            //kollar om redan besökt
            if (!visited.contains(neighbor)) {
                //markerar som besökt
                visited.insert(neighbor);
                //sparar det föräldern vi kom från
                parent[neighbor] = current;
                //lägger till granne i kön
                queue.push(neighbor);
            }
        }
    }

    //Hittar ordet som ligger längst bort
    string farthest = word;
    int longest_distance = 0;

    for (string current : visited) {
        int distance = 0;
        string temp = current;

        while (temp != word) {
            temp = parent[temp];
            distance++;
        }

        if (distance > longest_distance) {
            longest_distance = distance;
            farthest = current;
        }
    }

    //Börjar vid ordet längst bort och går tillbaks med parent
    string current = farthest;

    while (current != word) {
        result.push_back(current);
        current = parent[current];
    }

    //Lägger till startord
    result.push_back(word);

    return result;
}

/**
 * Läs in ordlistan och returnera den som en vector av orden. Funktionen läser även bort raden med
 * #-tecknet så att resterande kod inte behöver hantera det.
 */
Dictionary read_dictionary() {
    string line;

    //Skapar ett unordered_set istället för vector
    Dictionary result;

    while (std::getline(std::cin, line)) {
        if (line == "#")
            break;

        //Lägger till ordet i unordered_set istället.
        result.insert(line);
    }

    return result;
}

/**
 * Skriv ut en ordkedja på en rad.
 */
void print_chain(const vector<string> &chain) {
    if (chain.empty())
        return;

    vector<string>::const_iterator i = chain.begin();
    cout << *i;

    for (++i; i != chain.end(); ++i)
        cout << " -> " << *i;

    cout << endl;
}

/**
 * Skriv ut ": X ord" och sedan en ordkedja om det behövs. Om ordkedjan är tom, skriv "ingen lösning".
 */
void print_answer(const vector<string> &chain) {
    if (chain.empty()) {
        cout << "ingen lösning" << endl;
    } else {
        cout << chain.size() << " ord" << endl;
        print_chain(chain);
    }
}

/**
 * Läs in alla frågor. Anropar funktionerna "find_shortest" eller "find_longest" ovan när en fråga hittas.
 */
void read_questions(const Dictionary &dict) {
    string line;
    while (std::getline(std::cin, line)) {
        size_t space = line.find(' ');
        if (space != string::npos) {
            string first = line.substr(0, space);
            string second = line.substr(space + 1);
            vector<string> chain = find_shortest(dict, first, second);

            cout << first << " " << second << ": ";
            print_answer(chain);
        } else {
            vector<string> chain = find_longest(dict, line);

            cout << line << ": ";
            print_answer(chain);
        }
    }
}

int main() {
    Dictionary dict = read_dictionary();
    read_questions(dict);

    return 0;
}
