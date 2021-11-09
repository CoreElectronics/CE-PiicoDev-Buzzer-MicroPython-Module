# PiicoDev Buzzer melody example. Define a melody as a combination of notes and durations.

from PiicoDev_Buzzer import PiicoDev_Buzzer
from PiicoDev_Unified import sleep_ms

# Define some note-frequency pairs
notes = {'C4':262,
         'Db':277,
         'D' :294,
         'Eb':311,
         'E' :330,
         'F' :349,
         'Gb':370,
         'G' :392,
         'Ab':415,
         'A' :440,
         'Bb':466,
         'B' :494,
         'C5':523,
         'rest':0, # zero Hertz is the same as no tone at all
         }

# define a melody - two-dimensional list of notes and note-duration (ms)
melody = [['E',    500],
          ['D',    500],
          ['C4',   500],
          ['rest', 500],
          ['E',    500],
          ['D',    500],
          ['C4',   500],
          ['rest', 500],
          ['G',    500],
          ['F',    250],
          ['F',    250],
          ['E',    500],
          ['rest', 500],
          ['G',    500],
          ['F',    250],
          ['F',    250],
          ['E',    500],
          ['rest', 500],
          ]

buzz = PiicoDev_Buzzer(volume=2)

# play the melody
for x in melody:
    note = x[0] # extract the note name
    duration = x[1] # extract the duration
    buzz.tone(notes[note], duration)
    sleep_ms(duration)