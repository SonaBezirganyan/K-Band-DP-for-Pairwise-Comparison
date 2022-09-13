#include <bits/stdc++.h>

/**
 * An enum class to represent the types of alignments
 */
enum class Alignment {
    LOCAL,
    GLOBAL,
    SEMIGLOBAL
};

/**
 * A function to check whether or not the given position is inside the k-strip
 *
 * @param i represents the number of row
 * @param j represents the number of column
 * @param k represents the value of k
 * @return true if (i, j) is inside the k-strip, false otherwise
 */
bool is_inside_strip(int i, int j, int k);

/**
 * A function to compute the maximum value in a given matrix
 *
 * @param a represents the matrix
 * @return the value of the maximum entry in the matrix
 */
int get_max_entry(const std::vector<std::vector<int>> &a);


/**
 * A function to compute the maximum value in the last row of the given matrix
 *
 * @param a represents the matrix
 * @return the value of the maximum entry in the last row of the matrix
 */
int get_max_of_last_row(const std::vector<std::vector<int>> &a);

/**
 * A function to compute the maximum value in the last column of the given matrix
 *
 * @param a represents the matrix
 * @return the value of the maximum entry in the last column of the matrix
 */
int get_max_of_last_column(const std::vector<std::vector<int>> &a);

/**
 * A function to compute the score of an optimal alignment given the dp matrix
 *
 * @param a represents the matrix
 * @param alignment represents the type of the alignment
 * @return the score of an optimal alignment
 */
int get_score(const std::vector<std::vector<int>> &a, Alignment alignment);

/**
 * A function to construct and fill the dp matrix, given a fixed value of k
 *
 *
 * @param s represents the first sequence
 * @param t represents the second sequence
 * @param k represents the value of k
 * @param g represents the gap penalty
 * @param m represents the match score
 * @param p represents the mismatch penalty
 * @param alignment represents the type of the alignment
 * @return the dp matrix
 */
std::vector<std::vector<int>>
k_band(std::string s, std::string t, int k, int g, int m, int p, Alignment alignment);


/**
 * A function to determine the value of k and construct the corresponding k-band dp matrix
 *
 * @param s represents the first sequence
 * @param t represents the second sequence
 * @param g represents the gap penalty
 * @param m represents the match score
 * @param p represents the mismatch penalty
 * @param alignment represents the type of the alignment
 * @return a triple consisting of the score of an optimal alignment, the constructed matrix, and the value of k
 */
std::tuple<int, std::vector<std::vector<int>>, int>
construct_matrix(const std::string &s, const std::string &t, int g, int m, int p, Alignment alignment);


/**
 * A recursive function to perform the traceback to find all optimal alignemnts
 *
 * @param g represents the gap penalty
 * @param m represents the match score
 * @param p represents the mismatch penalty
 * @param results represents a vector to store the optimal alignments
 * @param x represents the row
 * @param y represents the column
 * @param k represents the value of k
 * @param s represents the first sequence
 * @param t represents the second sequence
 * @param rec1 represents a suffix of the current alignment
 * @param rec2 represents a suffix of the current alignment
 * @param dp represents the dp matrix
 * @param alignment represents the type of the alignment
 */
void reconstruct(int g, int m, int p, std::vector<std::pair<std::string, std::string>> &results, int x, int y, int k,
                 const std::string &s, const std::string &t, std::string rec1, std::string rec2,
                 const std::vector<std::vector<int>> &dp, Alignment alignment);


/**
 * A functiont to compute and print the optimal alignments between the given 2 sequences
 *
 * @param s represents the first sequence
 * @param t represents the second sequence
 * @param g represents the gap penalty
 * @param m represents the match score
 * @param p represents the mismatch penalty
 * @param alignment represents the type of the alignment
 */
void print_alignment(std::string s, std::string t, int g, int m, int p, Alignment alignment);
