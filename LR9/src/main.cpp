#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <set>

#define ll uint_fast64_t

const ll INF = std::numeric_limits<ll>::max();

typedef struct _s
{
    ll v2, w;
} s;


ll dijkstra(ll start, ll finish, const std::vector<std::vector<s>> &ETW, ll n, ll m)
{
    std::map<ll, ll> distFromStart;
    std::vector<bool> visited(n + 1, false);
    for (ll vert = 0; vert <= n; ++vert)
    {
        distFromStart[vert] = INF;
    }
    distFromStart[start] = 0;
    std::set<std::pair<ll, ll>> st;

    st.insert(std::make_pair(distFromStart[start], start));
    while (!st.empty())
    {
        ll vert = st.begin()->second;
        st.erase(st.begin());

        for (const auto &e : ETW[vert])
        {
            if (distFromStart[vert] + e.w < distFromStart[e.v2])
            {
                //st.erase(std::make_pair(distFromStart[e.v2], e.v2));
                distFromStart[e.v2] = distFromStart[vert] + e.w;
                visited[e.v2] = true;
                st.insert(std::make_pair(distFromStart[e.v2], e.v2));
            }
        }
    }

    return distFromStart[finish];
}

int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    ll n, m, start, finish;
    std::cin >> n >> m >> start >> finish;
    std::vector<std::vector<s>> edgeToWeight(n+1);

    ll v1, v2, w;
    for (ll i = 1; i <= m; ++i)
    {
        std::cin >> v1 >> v2 >> w;
        edgeToWeight[v1].push_back({v2, w});
        edgeToWeight[v2].push_back({v1, w});
    }

    ll res = dijkstra(start, finish, edgeToWeight, n, m);
    if (res == INF)
    {
        std::cout << "No solution\n";
    }
    else
    {
        std::cout << res << '\n';
    }

    return 0;
}


/*
5 6 1 5
1 2 2
1 3 0
3 2 10
4 2 1
3 4 4
4 5 5

*/
