# WEB CRAWL

from os import path, remove
from random import randint
from requests import get
from urllib.parse import urlparse

TOK = {'rodo'}
START = 'https://www.google.com'
FILE = 'alerts.dat'

if path.exists(FILE):
  remove(FILE)

seen = {}
q = [START]
alerts = 0

while len(q) > 0:
  # Check either next in queue or random
  opt = randint(1, 2)
  if opt == 1 or len(q) == 1:
    site = q[0]
    q = q[1:]
  else:
    i = randint(1, len(q)-1)
    site = q[i]
    q = q[:i] + q[i+1:]

  # Ensure valid url
  try:
    r = urlparse(site)
    r2 = all([r.scheme, r.netloc])
    if not r2:
      continue
  except ValueError:
    continue

  # Clean url
  while site.endswith('\\') or site.endswith('/'):
    site = site[:len(site)-1]

  # Check if seen, increase count
  if site in seen:
    seen[site] += 1
    continue
  else:
    seen[site] = 1

  # Fetch HTML
  print()
  print('Checking '+site)
  print(str(len(seen))+' seen, '+str(len(q))+' in queue, '+str(alerts)+\
      ' alerts')
  reply = get(site).text.lower()

  # Find tokens
  for t in TOK:
    i = reply.find(t)
    if i != -1:
      print('!!! ALERT !!!')
      alerts += 1
      with open(FILE, 'a') as f:
        f.write(site+'\n\n')
        f.write(reply[max(0,i-100):i]+'\n\n')
        f.write(reply[i:min(len(reply),i+100)]+'\n\n')
      break

  # Find links
  while True:
    i = reply.find('"https://')
    if i == -1:
      break
    reply = reply[i+1:]
    j = reply.find('"')
    if j == -1:
      break
    new = reply[:j]
    q += [new]
    reply = reply[j+1:]
