import time
import sys
import winsound

soundFile = 'D:\Navigation\Tools\LearningPython\Alarm\one_last_dance.wav'
not_executed = 1

def soundStart():
    print "in..."
    winsound.PlaySound(soundFile, winsound.SND_FILENAME)

while(not_executed):
    dt = list(time.localtime())
    hour = dt[3]
    minute = dt[4]
    if hour == 23 and minute == 59:
        soundStart()
        not_executed = 0