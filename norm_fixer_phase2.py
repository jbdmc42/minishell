#!/usr/bin/env python3
"""
Script to fix additional norminette errors in C files
Handles:
- Spaces on empty lines
- Missing space after keywords (if, while, for, switch)
- Too many consecutive spaces
"""
import os
import re

def fix_empty_lines(content):
    """Remove spaces on empty lines"""
    lines = content.split('\n')
    fixed_lines = []
    
    for line in lines:
        if line and line.strip() == '':
            fixed_lines.append('')
        else:
            fixed_lines.append(line)
    
    return '\n'.join(fixed_lines)

def fix_keyword_spacing(content):
    """Add space after keywords: if, while, for, switch"""
    lines = content.split('\n')
    fixed_lines = []
    
    keywords = ['if', 'while', 'for', 'switch', 'else']
    
    for line in lines:
        for keyword in keywords:
            # Only match keywords followed by ( without space
            pattern = r'\b' + keyword + r'\('
            replacement = keyword + ' ('
            line = re.sub(pattern, replacement, line)
        fixed_lines.append(line)
    
    return '\n'.join(fixed_lines)

def fix_consecutive_spaces(content):
    """Replace multiple consecutive spaces with single space (except indentation)"""
    lines = content.split('\n')
    fixed_lines = []
    
    for line in lines:
        # Preserve leading whitespace for indentation
        if line and line[0] in (' ', '\t'):
            # Find where indentation ends
            indent_end = len(line) - len(line.lstrip())
            indent = line[:indent_end]
            code = line[indent_end:]
            # Fix consecutive spaces in code part only
            code = re.sub(r'  +', ' ', code)
            fixed_lines.append(indent + code)
        else:
            # For lines without leading spaces, just fix spaces
            line = re.sub(r'  +', ' ', line)
            fixed_lines.append(line)
    
    return '\n'.join(fixed_lines)

def process_file(filepath):
    """Process a single C file"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Apply fixes
        content = fix_empty_lines(content)
        content = fix_keyword_spacing(content)
        content = fix_consecutive_spaces(content)
        
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        
        return True, "Fixed"
    except Exception as e:
        return False, str(e)

if __name__ == '__main__':
    target_files = [
        'parsing.c', 'parsing_helpers.c', 'parsing_helpers_two.c',
        'input_handling.c', 'tokenization.c', 'echo.c', 'exit.c',
        'quote_handling.c', 'utilities.c', 'utilities_two.c',
        'commands.c', 'setup_signal_handlers.c', 'main.c', 'cd.c',
        'unset.c', 'commands_extra.c', 'redir.c', 'pipes.c',
        'cleaning.c', 'export.c', 'ft_lsttochpp.c', 'pwd.c'
    ]
    
    for filename in target_files:
        filepath = f'/home/jbdmc/Documents/minishell/{filename}'
        if os.path.exists(filepath):
            success, msg = process_file(filepath)
            print(f"{filename}: {msg}" if success else f"{filename}: ERROR - {msg}")
