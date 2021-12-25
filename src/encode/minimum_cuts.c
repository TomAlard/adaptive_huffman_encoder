#include <string.h>
#include "minimum_cuts.h"
#include "../misc/util.h"


/*
 * Fill the minimum cuts array for every (i, j) < len in minimum_cuts[i][j].
 * If i > j we don't initialise the index, otherwise we set the index to j.
 * This way, each array minimum_cuts[i] is initialised with the most obvious (baseline) solution
 * of simply splitting the string at every character, as this is always valid.
 * Also initialize the first_cut_index array.
 */
void fill_arrays(int len, unsigned char minimum_cuts[][len], unsigned char first_cut_index[len]) {
    for (int i = 0; i < len; i++) {
        int j = i;
        while (j < len) {
            minimum_cuts[i][j] = j;
            j++;
        }
        first_cut_index[i] = i;
    }
}


// Returns the amount of cuts in the minimum cuts array at index i.
int amount_of_cuts(int len, unsigned char first_cut_index[len], unsigned char i) {
    return len - first_cut_index[i];
}


// Add a cut to the given array.
void add_cut(int len, unsigned char arr[len], unsigned char first_cut_index[len], int index) {
    first_cut_index[index]--;
    arr[first_cut_index[index]] = index;
}


// Remove all cuts from the array.
void set_no_cuts(int len, unsigned char arr[len], unsigned char first_cut_index[len], unsigned char start) {
    for (int i = start; i < len; i++) {
        arr[i] = 0;
    }
    first_cut_index[start] = len;
}


/*
 * Calculates the score used as tiebreaker in is_better_partition.
 * See is_better_partition's documentation for more information on the meaning of this score.
 */
int calculate_score(int len, unsigned char minimum_cuts[][len], const unsigned char first_cut_index[len],
                    unsigned char minimum_cuts_index, char cut_value[][len]) {
    int score = 0;
    unsigned char* cuts = minimum_cuts[minimum_cuts_index];
    unsigned char i = first_cut_index[minimum_cuts_index];
    while (i < len-1) {
        score += cut_value[cuts[i]][cuts[i+1] - 1];
        i++;
    }
    score += cut_value[cuts[i]][len-1];
    return score;
}


/*
 * One partition is considered better than another if it has less cuts.
 * If both have the same amount of cuts we use the values of the cuts as tiebreaker.
 * A value of 127 means we've already used this cut, while a value >= 0 means this cut is not in the tree.
 * It is preferable to not add new cuts if this does not reduce the amount of cuts in the partition,
 * so in this case the partition with the least amount of new cuts would be considered the better one.
 */
bool is_better_partition(int len, unsigned char minimum_cuts[][len], unsigned char first_cut_index[len],
                         unsigned char i, unsigned char j, char cut_value[][len]) {
    int cuts_old_partition = amount_of_cuts(len, first_cut_index, i);
    int cuts_new_partition = amount_of_cuts(len, first_cut_index, j) + 1;
    if (cuts_new_partition == cuts_old_partition) {
        int score_old_partition = calculate_score(len, minimum_cuts, first_cut_index, i, cut_value);
        int score_new_partition = calculate_score(len, minimum_cuts, first_cut_index, j, cut_value) + cut_value[i][j-1];
        return score_new_partition > score_old_partition;
    }
    return cuts_new_partition < cuts_old_partition;
}


/*
 * This problem is almost identical to this LeetCode problem:
 * https://leetcode.com/problems/palindrome-partitioning-ii/
 * With only two exceptions. The first one being the way we check for valid cuts,
 * in this case a cut is valid if it either has a length of 1 OR is present in the tree OR is
 * a commonly occurring substring and we thus wish to add it to the tree.
 * The second one is that we wish to return the indices at which we should
 * cut the string, instead of just returning the minimal amount of cuts needed.
 *
 * This method uses a divide and conquer approach as was proposed here:
 * https://www.techiedelight.com/find-minimum-cuts-needed-palindromic-partition-string/
 * We can also apply the same optimizations outlined in the article by first
 * checking the validity of all possible cuts and storing the results in the cut_value array.
 * This method implements the last proposed (bottom-up) solution in the article as
 * it has the lowest time and space complexity (both O(n^2)).
 */
unsigned char* find_minimum_cuts_indices(int len, char cut_value[][len], unsigned char* result_first_cut_index, int max_label_length) {
    // minimum_cuts[i] is the array of indices that indicate at which characters the
    // string should be cut to obtain a minimal partition for the string s[i..len-1].
    unsigned char minimum_cuts[len][len];
    unsigned char first_cut_index[len];
    fill_arrays(len, minimum_cuts, first_cut_index);
    for (int i = len-1; i >= 0; i--) {
        if (len-i <= max_label_length && cut_value[i][len-1] != -1) {
            // s[i..len-1] is a valid cut, so we do not have to split it further
            set_no_cuts(len, minimum_cuts[i], first_cut_index, i);
            add_cut(len, minimum_cuts[i], first_cut_index, i);
        } else {
            for (int j = i+1; j <= len && j-i <= max_label_length; j++) {
                if (cut_value[i][j-1] != -1 && is_better_partition(len, minimum_cuts, first_cut_index, i, j, cut_value)) {
                    memcpy(minimum_cuts[i], minimum_cuts[j], len);
                    first_cut_index[i] = first_cut_index[j];
                    add_cut(len, minimum_cuts[i], first_cut_index, i);
                }
            }
        }
    }
    // The result is contained in minimum_cuts[0], as we want the minimal partition for the entire string.
    unsigned char* result = safe_malloc(len);
    memcpy(result, minimum_cuts[0], len);
    *result_first_cut_index = first_cut_index[0];
    return result;
}


/*
 * Determine the values of all cuts. The value of cut_value[i][j] decides whether or not
 * the cut in the string from index i to index j (inclusive) is valid, and if it is, it's score.
 * A cut is considered valid if the result of should_encode_string for the label this cut produces is not -1.
 */
void find_cut_values(int len, char cut_value[][len], Tree* tree, Trie* trie, char* s, int max_label_length,
                     bool can_allocate_memory) {
    for (int i = 0; i < len; i++) {
        for (int j = i; j < len && j-i + 1 <= max_label_length; j++) {
            int label_length = j-i + 1;
            unsigned char* value = (unsigned char*) &s[i]; // reinterpret cast
            Label label = {value, label_length};
            cut_value[i][j] = should_encode_string(tree, trie, &label, can_allocate_memory);
        }
    }
}


/*
 * To determine how often each substring (<= max_label_length) occurs in the string s,
 * the method builds a trie containing all of these substrings and the amount of occurrences.
 * With this information the method can determine which cuts are valid using the find_cut_values method.
 * At this point we no longer need the trie, so we free it. Finally we pass the 2D array cut_value
 * to the find_minimum_cuts_indices method and return it's result.
 */
unsigned char* get_cuts(unsigned char* result_first_cut_index, Tree* tree, char* s, int s_length, int max_label_length,
                        bool can_allocate_memory) {
    char cut_value[s_length][s_length];
    Trie* trie = build_trie(s, s_length, max_label_length);
    find_cut_values(s_length, cut_value, tree, trie, s, max_label_length, can_allocate_memory);
    free_trie(trie);
    return find_minimum_cuts_indices(s_length, cut_value, result_first_cut_index, max_label_length);
}