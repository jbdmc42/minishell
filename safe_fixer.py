#!/usr/bin/env python3
"""
Safe norminette fixer that won't corrupt headers
"""
import os
import re

def fix_keyword_spacing(content):
    """Add space after keywords: if, while, for, switch"""
    lines = content.split('\n')
    fixed_lines = []
    
    keywords = ['if', 'while', 'for', 'switch']
    
    for line in lines:
        # Don't modify comments or strings
        if line.strip().startswith('/*') or line.strip().startswith('//'):
            fixed_lines.append(line)
            continue
            
        for keyword in keywords:
            # Only match keywords followed by ( without space, not inside strings
            if keyword + '(' in line and not line.strip().startswith('*'):
                line = line.replace(keyword + '(', keyword + ' (')
        
        fixed_lines.append(line)
    
    return '\n'.join(fixed_lines)

def fix_empty_lines(content):
    """Remove spaces on empty lines"""
    lines = content.split('\n')
    fixed_lines = []
    
    for line in lines:
        # If line has only whitespace, make it completely empty
        if line and not line.strip():
            fixed_lines.append('')
        else:
            fixed_lines.append(line)
    
    return '\n'.join(fixed_lines)

def process_file(filepath):
    """Process a single C file"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Apply fixes
        content = fix_keyword_spacing(content)
        content = fix_empty_lines(content)
        
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        
        return True, "OK"
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
            if not success:
                print(f"{filename}: ERROR - {msg}")
