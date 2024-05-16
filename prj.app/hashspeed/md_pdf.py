import markdown2
import pdfkit

# Read the Markdown file
with open('README.md', 'r') as file:
    markdown_content = file.read()

# Convert Markdown to HTML
html_content = markdown2.markdown(markdown_content)

# Save HTML to a temporary file
with open('temp.html', 'w') as file:
    file.write(html_content)

# Convert HTML to PDF
pdfkit.from_file('temp.html', 'output.pdf')

# Remove the temporary HTML file
import os
os.remove('temp.html')