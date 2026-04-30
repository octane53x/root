# SALTYBET

from selenium.webdriver import Chrome
from selenium.webdriver.chrome.service import Service
#from selenium.webdriver.chrome.options import Options

sv = Service(executable_path='C:/home/sw/chromedriver.exe')
br = Chrome(service=sv)
