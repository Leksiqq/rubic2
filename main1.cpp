#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <set>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> state;

map<char,int> color_name_to_code = {{'w',0},{'b',1},{'y',2},{'g',3},{'o',4},{'r',5}};

bool cmp_states(const state &x, const state &y){
    return (x.first < y.first) || (x.first == y.first && x.second < y.second);
}

void set_color(state &state, int color, int pos){
    if(pos / 12 == 0){
        state.first |= (color << pos * 3);
    }
    else{
        state.second |= (color << (pos % 12) * 3);
    }
}

int main()
{
    state start;
    string line;
    map<char, int> counts;
    string colours = "wbygor";
    string input;
    vector<string> finish_short = {
        "woyrgb",
        "wbygor",
        "wryobg",
        "wgybro",

        "oyrwgb",
        "obrgyw",
        "owrybg",
        "ogrbwy",

        "yrwogb",
        "ybwgro",
        "yowrbg",
        "ygwbor",

        "rwoygb",
        "rbogwy",
        "ryowbg",
        "rgobyw",

        "gobryw",
        "gwbyor",
        "grbowy",
        "gybwro",

        "bogrwy",
        "bygwor",
        "brgoyw",
        "bwgyro"
    };
    map<string, vector<int>> transformer = {
        {"front-cw",  { 2, 0, 3, 1,18,16, 6, 7, 8, 9,10,11,12,13,23,21,14,17,15,19,20, 4,22, 5}},
        {"front-ccw", { 1, 3, 0, 2,21,23, 6, 7, 8, 9,10,11,12,13,16,18, 5,17, 4,19,20,15,22,14}},
        {"back-cw",  { 0, 1, 2, 3, 4, 5,20,22,10, 8,11, 9,17,19,14,15,16, 7,18, 6,13,21,12,23}},
        {"back-ccw", { 0, 1, 2, 3, 4, 5,19,17, 9,11, 8,10,22,20,14,15,16,12,18,13, 6,21, 7,23}},
        {"right-cw",  { 0, 5, 2, 7, 4, 9, 6,11, 8,13,10,15,12, 1,14, 3,18,16,19,17,20,21,22,23}},
        {"right-ccw", { 0,13, 2,15, 4, 1, 6, 3, 8, 5,10, 7,12, 9,14,11,17,19,16,18,20,21,22,23}},
        {"left-cw",  {12, 1,14, 3, 0, 5, 2, 7, 4, 9, 6,11, 8,13,10,15,16,17,18,19,22,20,23,21}},
        {"left-ccw", { 4, 1, 6, 3, 8, 5,10, 7,12, 9,14,11, 0,13, 2,15,16,17,18,19,21,23,20,22}},
        {"top-cw",  {16,17, 2, 3, 4, 5, 6, 7, 8, 9,21,20,14,12,15,13,11,10,18,19, 0, 1,22,23}},
        {"top-ccw", {20,21, 2, 3, 4, 5, 6, 7, 8, 9,17,16,13,15,12,14, 0, 1,18,19,11,10,22,23}},
        {"bottom-cw",  { 0, 1,22,23, 6, 4, 7, 5,19,18,10,11,12,13,14,15,16,17, 2, 3,20,21, 9, 8}},
        {"bottom-ccw", { 0, 1,18,19, 5, 7, 4, 6,23,22,10,11,12,13,14,15,16,17, 9, 8,20,21, 2, 3}},
    };

    set<state, decltype(&cmp_states)> finish(&cmp_states);
    for(int i = 0; i < finish_short.size(); ++i){
        string s;
        for(int j = 0; j < 6; ++j){
            for(int k = 0; k < 4; ++k){
                s += finish_short[i][j];
            }
        }
        finish.insert(s);
    }
    for(int i = 0; i < 12; ++i){
        getline(cin, line);
        input += line + '\n';
        if(line.length() == 0){
            --i;
        }
        else {
            if(line.length() != 2){
                cout << "2 symbols expected! Got: " << input << endl;
                return 1;
            }
            for(int j = 0; j < 2; ++j){
                if(colours.find(line[j]) == string::npos){
                    cout << "Only '" << colours << "' symbols expected! Got: " << input << endl;
                    return 1;
                }
                ++counts[line[j]];
                if(counts[line[j]] > 4){
                    cout << "Each color must present 4 times! Got: " << input  << endl;
                    return 1;
                }
                start += line[j];
            }
        }
    }
    if(counts.size() != 6){
        cout << "Each color must present 4 times! Got: " << input  << endl;
        return 1;
    }
    for(auto it: counts){
        if(it.second != 4){
            cout << "Each color must present 4 times! Got: " << input  << endl;
            return 1;
        }
    }
    for(int i = 0; i < 24; ++i){
        cout << start[i];
        if(i % 4 == 3){
            cout << " ";
        }
    }
    cout << endl;

    map<state,pair<state,string>, decltype(&cmp_states)> prev(&cmp_states);
    map<state,int, decltype(&cmp_states)> dists(&cmp_states);
    vector<queue<string>> qu(1);

    qu[0].push(start);
    dists[start] = 0;

    int ans = -1;
    state ans_finish;

    for(int i = 0; ans == -1 && i < qu.size(); ++i){
        while(ans == -1 && !qu[i].empty()){
            string cur = qu[i].front();
            qu[i].pop();
            if(dists[cur] == i){
                for(auto tr: transformer){
                    string nb;
                    for(int j = 0; j < 24; ++j){
                        nb += cur[tr.second[j]];
                    }
                    if(dists.find(nb) == dists.end()){
                        dists[nb] = i + 1;
                        if(qu.size() - 1 < i + 1){
                            qu.resize(i + 2);
                        }
                        qu[i + 1].push(nb);
                        prev[nb] = {cur, tr.first};
                        if(finish.find(nb) != finish.end()){
                            ans = i + 1;
                            ans_finish = nb;
                            break;
                        }
                    }
                }
            }
        }
//        break;
    }

    if(ans == -1){
        cout << "Input state is unreachable!" << endl;
        return 1;
    }

    stack<string> solve;
    string cur = ans_finish;
    while(!cur.empty()){
        auto it = prev.find(cur);
        if(it == prev.end()){
            break;
        }
        solve.push(it->second.second);
        cur = it->second.first;
    }

    while(!solve.empty()){
        cout << solve.top() << endl;
        solve.pop();
    }

    for(int i = 0; i < 24; ++i){
        cout << ans_finish[i];
        if(i % 4 == 3){
            cout << " ";
        }
    }
    cout << endl;

    return 0;
}
