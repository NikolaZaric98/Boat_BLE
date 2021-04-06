# -*- coding: utf-8 -*-
"""
Created on Tue Sep  1 23:24:12 2020

@author: nikol
"""
# Ukoliko se koristi bilo koja vrsta prepoznavanja osim sphinix
# potrebno je da postoji internet konekcija
import speech_recognition as sr
import pyaudio

r=sr.Recognizer()
mic=sr.Microphone()
try:
        
    with mic as source:
        audio=r.listen(source)
    print(r.recognize_google(audio))
except sr.UnknownValueError:
    print('Nije zabelezeno')