#!/usr/bin/env python3
"""
Script to fix common norminette errors in C files
"""
import os
import re
import sys

def fix_inline_comments(content):
    """Remove inline comments that violate norm (comments after code on same line)"""
    lines = content.split('\n')
    fixed_lines = []
    
    for line in lines:
        # Skip if line is a comment-only line or empty
        if line.strip().startswith('/*') or line.strip().startswith('//') or not line.strip():
            fixed_lines.append(line)
            continue
        
        # Check for inline comments (// style)
        if '//' in line and not line.strip().startswith('//'):
            # Remove the inline comment
            line = line[:line.index('//')].rstrip()
        
        fixed_lines.append(line)
    
    return '\n'.join(fixed_lines)

def replace_spaces_with_tabs(content):
    """Replace leading spaces with tabs (for indentation only)"""
    lines = content.split('\n')
    fixed_lines = []
    
    for line in lines:
        if line and line[0] == ' ':
            # Count leading spaces
            leading_spaces = len(line) - len(line.lstrip(' '))
            # Replace groups of 4 spaces with tabs, but be careful
            # Convert to tabs (assuming 4 spaces = 1 tab)
            tabs_count = leading_spaces // 4
            remaining_spaces = leading_spaces % 4
            new_line = '\t' * tabs_count + ' ' * remaining_spaces + line.lstrip(' ')
            fixed_lines.append(new_line)
        else:
            fixed_lines.append(line)
    
    return '\n'.join(fixed_lines)

def process_file(filepath):
    """Process a single C file"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Apply fixes
        content = fix_inline_comments(content)
        content = replace_spaces_with_tabs(content)
        
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
        'unset.c', 'commands_extra.c'
    ]
    
    for filename in target_files:
        filepath = f'/home/jbdmc/Documents/minishell/{filename}'
        if os.path.exists(filepath):
            success, msg = process_file(filepath)
            print(f"{filename}: {msg}" if success else f"{filename}: ERROR - {msg}")
