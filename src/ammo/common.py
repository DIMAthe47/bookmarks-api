import random,string

def random_word(max_length):
   random_length=random.randint(1, max_length)
   return ''.join(random.choice(string.lowercase) for i in range(random_length))