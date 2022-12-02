DP = {}

def count_win(player1, player2, score1, score2):
  if score1 >= 21:
    return (1,0)
  if score2 >= 21:
    return (0, 1)

  if (player1, player2, score1, score2) in DP:
    return DP[(player1, player2, score1, score2)]

  ans = (0,0)
  for d1 in [1,2,3]:
    for d2 in [1,2,3]:
      for d3 in [1,2,3]:
        new_player1 = (player1 + d1 + d2 + d3) % 10

        x, y = count_win(player2, new_player1, score2, score1 + new_player1 + 1)
        ans = (ans[0] + y, ans[1] + x)

  DP[(player1, player2, score1, score2)] = ans
  return ans

print(max(count_win(3, 2, 0, 0)))