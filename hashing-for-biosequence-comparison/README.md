The goal of this lab is to implement hashing as part of a tool for comparing genomic DNA sequences in C++. The approach to biosequence comparison that we’ll use here is an important part of such well-known tools as FASTA (Pearson & Lipman 1988) and BLAST (Altschul et al. 1990, 1997; Altschul & Gish 1996).

Naively, we could find k-mers (i.e. substrings of length k) common to s1 and s2 by comparing every k-mer from one sequence to every k-mer from the other. Such an approach would take time Θ(|s1|·|s2|), which is unacceptable because interesting DNA sequences range from thousands to billions of characters in length.

But here I implement a much more efficient sequence comparison algorithm based on hashing. Call s1 the corpus string and s2 the pattern string, and assume we are searching for common substrings of length k. We first construct a table T of every k-mer in the pattern string, remembering where in the pattern it occurs. Then, for each k-mer in the corpus string, we check whether it occurs in the table T; if so, we have found a match between pattern and corpus. If T supports constant-time insertions and lookups (e.g., if it is a hash table), we can process the entire pattern and corpus in time Θ(|s1| + |s2| + M),where M is the number of common substrings actually found. In general, this time cost is much lower than the cost of the naive algorithm. Fast substring matching based on hashing therefore forms the core of many of today’s high-speed biosequence matching algorithms.

To make things slightly more interesting,I also allow the user to specify a mask string that contains “uninteresting” DNA. For example, if we’re looking for matching genes, we might not be interested in any common substrings that are part of known repeats. Any k-mer appearing in the mask string is removed from the table before searching for matches in the corpus.

Besides the basic hashing, I also Implement the following improvements to the hash table:

1. Extend each slot of the hash table to store the integer result of applying the toHashValue procedure to the key of the Record in that slot. Use this extra value to speed up searches by avoiding a full comparison of two strings when their hash values differ.

2. Implement the doubling procedure to allow the hash table to grow dynamically as records are added. 
