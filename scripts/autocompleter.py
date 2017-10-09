import os.path
import readline

class Completer(object):

    def _match_path(self, text):
        text = readline.get_line_buffer()
        return DirectoryPathCompleter(text).completions()

    def complete(self, text, state):
        matches = self._match_path(text)
        try:
            return matches[state]
        except IndexError:
            return None


class DirectoryPathCompleter(object):

    def __init__(self, text):
        self.text = text

    def completions(self):
        path, rest = os.path.split(self.text)
        if rest == '.' or rest == '..':
            result = [rest + '/']
        else:
            result = [i + '/' for i in get_dirs_under(path) if i.startswith(rest)]
        return result


def get_dirs_under(path):
    if not path:
        path = '.'
    dirpath, dirnames, filenames = os.walk(path).next()
    return dirnames



class ListCompleter(object):  # Custom completer

    def __init__(self, options):
        self.options = sorted(options)

    def complete(self, text, state):
        if state == 0:  # on first trigger, build possible matches
            if text:  # cache matches (entries that start with entered text)
                self.matches = [s for s in self.options
                                    if s and s.startswith(text)]
            else:  # no text entered, all matches possible
                self.matches = self.options[:]
        try:
            return self.matches[state]
        except IndexError:
            return None
