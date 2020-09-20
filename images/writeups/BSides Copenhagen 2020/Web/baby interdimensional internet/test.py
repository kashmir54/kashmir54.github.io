'''
global garage
garage = {}
try: exec('print(dir(), "=")', garage)
except: pass

print(garage)
'''

def calc(recipe):
	global garage
	garage = {}
	try: exec(recipe, garage)
	except: pass


ingredient = "print('"
recipe = '%s = %s' % (ingredient, "')")

calc(recipe)

if garage.get(ingredient, ''):
	print(garage)

