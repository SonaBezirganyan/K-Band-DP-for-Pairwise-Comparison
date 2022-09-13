#include <bits/stdc++.h>
#include "k_band_dp.h"

bool is_inside_strip(int i, int j, int k) {
    return std::abs(i - j) <= k;
}

int get_max_entry(const std::vector<std::vector<int>> &a) {
    int max = a[0][0];
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < a[i].size(); ++j) {
            max = std::max(max, a[i][j]);
        }
    }

    return max;
}

int get_max_of_last_row(const std::vector<std::vector<int>> &a) {
    int n = a[0].size();
    int max = a[n - 1][0];
    for (int i = 0; i < n; ++i) {
        max = std::max(max, a[n - 1][i]);
    }

    return max;
}

int get_max_of_last_column(const std::vector<std::vector<int>> &a) {
    int n = a.size();
    int max = a[0][n - 1];
    for (int i = 0; i < n; ++i) {
        max = std::max(max, a[i][n - 1]);
    }

    return max;
}

int get_score(const std::vector<std::vector<int>> &a, Alignment alignment) {
    int n = a.size() - 1;
    if (alignment == Alignment::GLOBAL) {
        return a[n][n];
    }
    if (alignment == Alignment::LOCAL) {
        return get_max_entry(a);
    }
    return std::max(get_max_of_last_column(a), get_max_of_last_row(a)); // semiglobal
}

std::vector<std::vector<int>> k_band(std::string s, std::string t, int k, int g, int m, int p, Alignment alignment) {
    assert(s.size() == t.size());
    int n = s.size();
    std::vector<std::vector<int>> a(n + 1, std::vector<int>(n + 1));
    if (alignment == Alignment::GLOBAL) {
        for (int i = 0; i <= k; ++i) {
            a[i][0] = i * g;
            a[0][i] = i * g;
        }
    }
    for (int i = 1; i <= n; ++i) {
        for (int d = -k; d <= k; ++d) {
            int j = i + d;
            if (j >= 1 && j <= n) {
                a[i][j] = a[i - 1][j - 1];
                if (s[i - 1] == t[j - 1]) {
                    a[i][j] += m;
                } else {
                    a[i][j] += p;
                }
                if (is_inside_strip(i - 1, j, k)) {
                    a[i][j] = std::max(a[i][j], a[i - 1][j] + g);
                }
                if (is_inside_strip(i, j - 1, k)) {
                    a[i][j] = std::max(a[i][j], a[i][j - 1] + g);
                }
                if (alignment == Alignment::LOCAL) {
                    a[i][j] = std::max(a[i][j], 0);
                }
            }
        }

    }

    return a;
}

std::tuple<int, std::vector<std::vector<int>>, int>
construct_matrix(const std::string &s, const std::string &t, int g, int m, int p, Alignment alignment) {
    int n = s.size();
    int k = 1;
    while (true) {
        std::vector<std::vector<int>> dp = k_band(s, t, k, g, m, p, alignment);
        int score = get_score(dp, alignment);
        if (k * (m - 2 * g) >= m * n - score - m + 2 * g) {
            std::vector<std::pair<std::string, std::string>> result;
            int l = 2 * n;
            std::string align_s = "", align_t = "";

            for (int i = 0; i < l; i++) {
                align_s += " ";
                align_t += " ";
            }
            return std::make_tuple(score, dp, k);
        }
        k *= 2;
    }
}


void reconstruct(int g, int m, int p, std::vector<std::pair<std::string, std::string>> &results, int x, int y, int k,
                 const std::string &s, const std::string &t, std::string rec1, std::string rec2,
                 const std::vector<std::vector<int>> &dp, Alignment alignment) {

    if (x - 1 >= 0 && is_inside_strip(y, x - 1, k) && dp[y][x] == (dp[y][x - 1] + g)) {
        reconstruct(g, m, p, results, x - 1, y, k, s, t, "-" + rec1, t[x - 1] + rec2, dp, alignment);
    }

    if (y - 1 >= 0 && is_inside_strip(y - 1, x, k) && dp[y][x] == (dp[y - 1][x] + g)) {
        reconstruct(g, m, p, results, x, y - 1, k, s, t, s[y - 1] + rec1, "-" + rec2, dp, alignment);
    }

    if (x - 1 >= 0 && y - 1 >= 0 && is_inside_strip(y - 1, x - 1, k) && s[y - 1] == t[x - 1] &&
        (dp[y][x] == dp[y - 1][x - 1] + m)) {
        reconstruct(g, m, p, results, x - 1, y - 1, k, s, t, s[y - 1] + rec1, t[x - 1] + rec2, dp, alignment);
    }

    if (x - 1 >= 0 && y - 1 >= 0 && is_inside_strip(y - 1, x - 1, k) && s[y - 1] != t[x - 1] &&
        (dp[y][x] == dp[y - 1][x - 1] + p)) {
        reconstruct(g, m, p, results, x - 1, y - 1, k, s, t, s[y - 1] + rec1, t[x - 1] + rec2, dp, alignment);
    }

    if (alignment == Alignment::LOCAL && dp[y][x] == 0) {
        results.push_back(std::make_pair(rec1, rec2));
    }
    if (alignment != Alignment::LOCAL && x == 0 && y == 0) {
        results.push_back(std::make_pair(rec1, rec2));
    }
}

void print_alignment(std::string s, std::string t, int g, int m, int p, Alignment alignment) {

    std::tuple<int, std::vector<std::vector<int>>, int> dp_tuple = construct_matrix(s, t, g, m, p, alignment);

    std::vector<std::vector<int>> dp_ = std::get<1>(dp_tuple);
    int score = std::get<0>(dp_tuple);


    std::vector<std::pair<std::string, std::string>> results;

    if (alignment == Alignment::GLOBAL) {
        reconstruct(g, m, p, results, dp_[0].size() - 1, dp_.size() - 1, std::get<2>(dp_tuple), s, t, "", "", dp_,
                    Alignment::GLOBAL);
    } else if (alignment == Alignment::LOCAL) {
        for (int i = 0; i < dp_.size(); i++) {
            for (int j = 0; j < dp_.size(); j++) {
                if (dp_[i][j] == score) {
                    reconstruct(g, m, p, results, i, j, std::get<2>(dp_tuple), s, t, "", "", dp_, Alignment::LOCAL);
                }
            }
        }
    } else {    // semiglobal
        int n = dp_.size();
        for (int i = 0; i < n; i++) {
            if (dp_[n - 1][i] == score) {
                reconstruct(g, m, p, results, dp_[0].size() - 1, i, std::get<2>(dp_tuple), s, t, "", "", dp_,
                            Alignment::SEMIGLOBAL);
            }
        }
        n = dp_[0].size();
        for (int i = 0; i < n; i++) {
            if (dp_[i][n - 1] == score) {
                reconstruct(g, m, p, results, i, dp_.size() - 1, std::get<2>(dp_tuple), s, t, "", "", dp_,
                            Alignment::SEMIGLOBAL);
            }
        }
    }


    std::sort(results.begin(), results.end());
    for (auto x: results) {
        std::cout << x.first << "\n";
        std::cout << x.second << "\n\n";
        // break;
    }

}


int main() {
    std::cout << "Enter the first sequence.\n";
    std::string s, t;
    std::cin >> s;
    std::cout << "Enter the second sequence.\n";
    std::cin >> t;
    int gap_penalty;
    std::cout << "Enter the gap penalty.\n";
    std::cin >> gap_penalty;
    std::string alignment;
    std::cout << "Enter the type of the alignment: GLOBAL, LOCAL, or SEMIGLOBAL\n";
    std::cin >> alignment;
    Alignment a;
    if (alignment == "GLOBAL") {
        a = Alignment::GLOBAL;
    } else if (alignment == "LOCAL") {
        a = Alignment::LOCAL;
    } else {
        a = Alignment::SEMIGLOBAL;
    }

    print_alignment(s, t, gap_penalty, 1, -1, a);

    std::tuple<int, std::vector<std::vector<int>>, int> res = construct_matrix(s, t, gap_penalty, 1, -1, a);

    for (int i = 0; i < std::get<1>(res).size(); i++) {
        for (int j = 0; j < std::get<1>(res)[0].size(); j++) {
            std::cout << std::get<1>(res)[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nThe optimal score is " << std::get<0>(res) << std::endl;

}