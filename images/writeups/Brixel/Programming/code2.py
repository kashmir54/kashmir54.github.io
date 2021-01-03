x = 1
y = 1
prev_ans = 1

while x != 526:
	ans = x*y+prev_ans+3
	print('{} = {} * {} + {} + 3'.format(ans, x, y, prev_ans))
	x += 1
	y += 1
	prev_ans = ans

print(ans)