#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <ctime>

using namespace std;
typedef long long ll;
typedef pair<ll,ll> state;
typedef pair<int,int> one_move;

#define FRONT 1
#define BACK 2
#define RIGHT 3
#define LEFT 4
#define TOP 5
#define BOTTOM 6
#define CLOCKWISE (-1)
#define COUNTERCLOCKWISE 1

map<char,int> color_name_to_code = {{'w',0},{'b',1},{'y',2},{'g',3},{'o',4},{'r',5}};
string colours = "wbygor";

void set_color(state &st, ll color, int pos){
    if(pos / 12 == 0){
        st.first |= (color << pos * 3);
    }
    else{
        st.second |= (color << (pos % 12) * 3);
    }
}

int get_color(state &st, int pos){
    if(pos / 12 == 0){
        return (st.first & (7ll << pos * 3)) >> pos * 3;
    }
    else{
        return (st.second & (7ll << (pos % 12) * 3)) >> (pos % 12) * 3;
    }
}

void print_state(state &st){
    for(int i = 0; i < 24; ++i){
        cout << colours[get_color(st, i)];
        if(i % 4 == 3){
            cout << " ";
        }
    }
}

void transform_state(state &st, state &target, vector<int> &transformer){
    for(int i = 0; i < 24; ++i){
        set_color(target, get_color(st, transformer[i]), i);
    }
}

string one_move_to_string(one_move &m){
    return (
        m.first == FRONT ? (string)"front" :
        (
            m.first == BACK ? (string)"back" :
            (
                m.first == RIGHT ? (string)"right" :
                (
                    m.first == LEFT ? (string)"left" :
                    (
                        m.first == TOP ? (string)"top" : (string)"bottom"
                    )
                )
            )
        )
    )
    + (string)"-" + (m.second == CLOCKWISE ? (string)"cw" : (string)"ccw");
}

int main(int argc, char**argv)
{
    state start;
    string line;
    map<char, int> counts;
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
    map<one_move, vector<int>> transformer = {
        {{FRONT, CLOCKWISE},         { 2, 0, 3, 1,18,16, 6, 7, 8, 9,10,11,12,13,23,21,14,17,15,19,20, 4,22, 5}},
        {{FRONT, COUNTERCLOCKWISE},  { 1, 3, 0, 2,21,23, 6, 7, 8, 9,10,11,12,13,16,18, 5,17, 4,19,20,15,22,14}},
        {{BACK, CLOCKWISE},          { 0, 1, 2, 3, 4, 5,20,22,10, 8,11, 9,17,19,14,15,16, 7,18, 6,13,21,12,23}},
        {{BACK, COUNTERCLOCKWISE},   { 0, 1, 2, 3, 4, 5,19,17, 9,11, 8,10,22,20,14,15,16,12,18,13, 6,21, 7,23}},
        {{RIGHT, CLOCKWISE},         { 0, 5, 2, 7, 4, 9, 6,11, 8,13,10,15,12, 1,14, 3,18,16,19,17,20,21,22,23}},
        {{RIGHT, COUNTERCLOCKWISE},  { 0,13, 2,15, 4, 1, 6, 3, 8, 5,10, 7,12, 9,14,11,17,19,16,18,20,21,22,23}},
        {{LEFT, CLOCKWISE},          {12, 1,14, 3, 0, 5, 2, 7, 4, 9, 6,11, 8,13,10,15,16,17,18,19,22,20,23,21}},
        {{LEFT, COUNTERCLOCKWISE},   { 4, 1, 6, 3, 8, 5,10, 7,12, 9,14,11, 0,13, 2,15,16,17,18,19,21,23,20,22}},
        {{TOP, CLOCKWISE},           {16,17, 2, 3, 4, 5, 6, 7, 8, 9,21,20,14,12,15,13,11,10,18,19, 0, 1,22,23}},
        {{TOP, COUNTERCLOCKWISE},    {20,21, 2, 3, 4, 5, 6, 7, 8, 9,17,16,13,15,12,14, 0, 1,18,19,11,10,22,23}},
        {{BOTTOM, CLOCKWISE},        { 0, 1,22,23, 6, 4, 7, 5,19,18,10,11,12,13,14,15,16,17, 2, 3,20,21, 9, 8}},
        {{BOTTOM, COUNTERCLOCKWISE}, { 0, 1,18,19, 5, 7, 4, 6,23,22,10,11,12,13,14,15,16,17, 9, 8,20,21, 2, 3}},
    };

    char c;
    for(int i = 0; i < 24; ++i){
        cin >> c;
        if(colours.find(c) == string::npos){
            cout << "Only '" << colours << "' symbols expected! Got: " << input << endl;
            return 1;
        }
        ++counts[c];
        if(counts[c] > 4){
            cout << "Each color must present 4 times! Got: " << input  << endl;
            return 1;
        }
        set_color(start, color_name_to_code[c], i);
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
    print_state(start);
    cout << endl;

    int ans = -1;
    state ans_finish;

    map<state,int> dists0;
    vector<queue<state>> qu0(1);

    for(int i = 0; i < finish_short.size(); ++i){
        state s;
        int pos = 0;
        for(int j = 0; j < 6; ++j){
            for(int k = 0; k < 4; ++k){
                set_color(s, color_name_to_code[finish_short[i][j]], pos);
                ++pos;
            }
        }
        if(s.first == start.first && s.second == start.second){
            ans = 0;
            ans_finish = start;
            break;
        }
        qu0[0].push(s);
        dists0[s] = 0;
    }

    time_t started = time(nullptr);

    if(ans == -1){
        map<state,pair<state,one_move>> prev0;
        map<state,pair<state,one_move>> prev;
        map<state,int> dists;
        vector<queue<state>> qu(1);

        qu[0].push(start);
        dists[start] = 0;

        for(int i = 0; ans == -1 && i < max(qu.size(), qu0.size()); ++i){
            while(ans == -1 && !qu0[i].empty()){
                state cur0 = qu0[i].front();
                qu0[i].pop();
                if(dists0[cur0] == i){
                    for(auto tr: transformer){
                        state nb;
                        transform_state(cur0, nb, tr.second);
                        if(dists0.find(nb) == dists0.end()){
                            dists0[nb] = i + 1;
                            if(qu0.size() - 1 < i + 1){
                                qu0.resize(i + 2);
                            }
                            qu0[i + 1].push(nb);
                            prev0[nb] = {cur0, tr.first};
                        }
                        if(dists.find(nb) != dists.end()){
                            ans = dists[nb] + i + 1;
                            ans_finish = nb;
                            break;
                        }
                    }
                }
            }
            while(ans == -1 && !qu[i].empty()){
                state cur = qu[i].front();
                qu[i].pop();
                if(dists[cur] == i){
                    for(auto tr: transformer){
                        state nb;
                        transform_state(cur, nb, tr.second);
                        if(dists.find(nb) == dists.end()){
                            dists[nb] = i + 1;
                            if(qu.size() - 1 < i + 1){
                                qu.resize(i + 2);
                            }
                            qu[i + 1].push(nb);
                            prev[nb] = {cur, tr.first};
                        }
                        if(dists0.find(nb) != dists0.end()){
                            ans = dists0[nb] + i + 1;
                            ans_finish = nb;
                            break;
                        }
                    }
                }
            }
        }
        if(ans == -1){
            cout << "Input state is unreachable!" << endl;
            return 1;
        }

        cout << ans << " moves" << endl;

        stack<one_move> solve;
        state cur = ans_finish;
        while(true){
            auto it = prev.find(cur);
            if(it == prev.end()){
                break;
            }
            solve.push(it->second.second);
            cur = it->second.first;
        }

        while(!solve.empty()){
            cout << one_move_to_string(solve.top()) << endl;
            solve.pop();
        }

        cur = ans_finish;
        while(true){
            auto it = prev0.find(cur);
            if(it == prev0.end()){
                print_state(cur);
                cout << endl;
                break;
            }
            it->second.second.second *= -1;
            cout << one_move_to_string(it->second.second) << endl;
            cur = it->second.first;
        }
    }

    time_t finished = time(nullptr);

    cout << "seconds elapsed: " << finished - started << endl;

    return 0;
}
