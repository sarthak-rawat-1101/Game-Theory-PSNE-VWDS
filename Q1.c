/*
# Introduction to Game Theory, Programming Assignment
## Team: Common Knowledge
## Members: Anchal Jakhmola, Bhupendra Sharma, Jatin Gupta, Sarthak Rawat
## Task: Find PSNE and Very Weakly Dominant Strategy.
## Input format:
- First line containe 'N', an integer denoting number of players.
- Second line contains 'N' space seperated integers, denoting number of \
  strategies of each player.
- Third line contains utility matrix in NFG format.
## Output format:
- Number denoting count of PSNE profiles, 'N_psne'.
- Next 'N_psne' lines contain 'N' space seperated numbers each, \
  denoting a strategy profile that is PSNE.
- Next 'N' lines contain very weakly dominant strategy for each player. \
  Player 1 to Player 'N'.
*/

#include <limits.h>
#include <stdio.h>

int get_idx(int n, int size[], int idxs[], int ignore) {
  int idx = 0, i, prev = 1;
  for (i = 0; i < n; ++i) {
    if (i == ignore)
      continue;
    idx += prev * idxs[i];
    prev *= size[i];
  }
  return idx;
}

void inc(int S[], int N, int idx[]) {
  int i;
  for (i = 0; i < N; ++i) {
    idx[i] += 1;
    if (idx[i] >= S[i])
      idx[i] = 0;
    else
      break;
  }
}

void upd_max(int arr[], int idx, int val) {
  if (arr[idx] < val)
    arr[idx] = val;
}

int main(void) {
  int N, i, j, prod_S = 1, si, s_i, profile, player;
  int *S, *itr, ***U, **mx, util, sei, se_i;
  int count_s, n_vwds, n_psne = 0, *buffer;
  scanf("%d", &N);
  itr = (int *)malloc(N * sizeof(int));
  mx = (int **)malloc(N * sizeof(int *));
  S = (int *)malloc(N * sizeof(int));
  U = (int ***)malloc(N * sizeof(int **));
  for (i = 0; i < N; ++i) {
    scanf("%d", S + i);
    prod_S *= S[i];
  }
  buffer = (int *)malloc(prod_S * sizeof(int));
  for (i = 0; i < N; ++i) {
    U[i] = (int **)malloc(S[i] * sizeof(int *));
    for (si = 0; si < S[i]; ++si)
      U[i][si] = (int *)malloc(prod_S / S[i] * sizeof(int));
  }
  for (i = 0; i < N; ++i) {
    mx[i] = (int *)malloc((prod_S / S[i]) * sizeof(int));
    for (j = 0; j < (prod_S / S[i]); ++j)
      mx[i][j] = INT_MIN;
  }
  memset(itr, 0, N * sizeof(int));
  for (i = 0; i < prod_S; ++i) {
    for (player = 0; player < N; ++player) {
      scanf("%d", &util);
      U[player][itr[player]][get_idx(N, S, itr, player)] = util;
      upd_max(mx[player], get_idx(N, S, itr, player), util);  
    }
    inc(S, N, itr);
  }
  // printf("------PSNE--------\n");
  memset(itr, 0, N * sizeof(int));
  for (profile = 0; profile < prod_S; ++profile) {
    for (i = 0; i < N; ++i) {
      sei = itr[i];
      se_i = get_idx(N, S, itr, i);
      for (si = 0; si < S[i]; ++si)
        if (U[i][si][se_i] > U[i][sei][se_i])
          break;
      if (si != S[i])
        break;
    }
    if (i == N)
      buffer[n_psne++] = profile;
    inc(S, N, itr);
  }
  printf("%d\n", n_psne);
  for (i = 0; i < n_psne; ++i) {
    profile = buffer[i];
    for (j = 0; j < N; ++j) {
      printf("%d ", (profile % S[j]) + 1);
      profile /= S[j];
    }
    printf("\n");
  }
  // printf("------VWDS--------\n");
  for (i = 0; i < N; ++i) {
    n_vwds = 0;
    for (si = 0; si < S[i]; ++si) {
      for (s_i = 0; s_i < prod_S / S[i]; ++s_i)
        if (U[i][si][s_i] < mx[i][s_i])
          break;
      if (s_i == prod_S / S[i])
        buffer[n_vwds++] = si + 1;
    }
    printf("%d ", n_vwds);
    for (si = 0; si < n_vwds; ++si)
      printf("%d ", buffer[si]);
    printf("\n");
  }
  return 0;
}