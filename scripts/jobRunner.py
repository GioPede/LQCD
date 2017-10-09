import readline
import autocompleter
import pgfg

readline.parse_and_bind('tab: complete')
readline.set_completer_delims(' \t\n')

# Ask for the program to run
programCompleter = autocompleter.ListCompleter(['pgfg', 'gfr', 'gfe', 'flow', 'help'])
readline.set_completer(programCompleter.complete)
program = raw_input('Select Program\t\t  > ')

# PGFG
if program == 'pgfg':
    pgfg.runPgfg()
