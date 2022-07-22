import re
from pandas.io.clipboard import clipboard_get, clipboard_set

# Copy note from OneNote to Obsidian, and then copy Obsidian
# Do not copy images
page = clipboard_get()

# Fix newlines
page = re.sub('\n\n', '\n', page)
page = re.sub(r'^#', '\n#', page, flags=re.MULTILINE)

# Fix lists
page = re.sub(r'^-   ', r'- ', page, flags=re.MULTILINE)
page = re.sub(r'^(\d)\.  ', r'\1. ', page, flags=re.MULTILINE)

# Titles
if re.search(r'^## ', page, flags=re.MULTILINE) is None:
    page = re.sub(r'^#', '', page, flags=re.MULTILINE)

# Time marker
page = re.sub(r'^t(?:\d{6}|\d{4}|\d{2}|~)(?: \S+)?$', r'\n<!--\g<0>-->', page, flags=re.MULTILINE)

clipboard_set(page)