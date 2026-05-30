#!/usr/bin/env python3
"""
Safe norm error fixer - fixes common norm violations without breaking code
"""
import os
import re

def fix_file(filepath):
    """Fix a single file"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original = content
        lines = content.split('\n')
        fixed_lines = []
        in_header = True
        
        for i, line in enumerate(lines):
            # Skip the 42 header block
            if i < 15 and ('/*' in line or '*' in line):
                fixed_lines.append(line)
                continue
            
            in_header = False
            
            # Remove inline comments (but not comment-only lines)
            if '//' in line and not line.strip().startswith('//') and not line.strip().startswith('/*'):
                # This is an inline comment, remove it
                line = line[:line.index('//')].rstrip()
            
            # Fix indentation: replace leading spaces with tabs
            if line and line[0] == ' ' and not line.strip().startswith('*'):
                # Count leading spaces
                leading_spaces = len(line) - len(line.lstrip(' '))
                # Convert groups of 4 spaces to tabs
                tabs = leading_spaces // 4
                remaining = leading_spaces % 4
                if tabs > 0 or (remaining == 0 and leading_spaces > 0):
                    # Only convert if we have full groups of 4 or exact multiples
                    new_indent = '\t' * tabs + ' ' * remaining
                    line = new_indent + line.lstrip(' ')
            
            # Remove trailing spaces on empty lines
            if line.strip() == '':
                line = ''
            
            fixed_lines.append(line)
        
        content = '\n'.join(fixed_lines)
        
        # Only write if changes were made
        if content != original:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            return True
        return False
        
    except Exception as e:
        print(f"Error processing {filepath}: {e}")
        return False

if __name__ == '__main__':
    c_files = [
        'parsing.c', 'parsing_helpers.c', 'parsing_helpers_two.c',
        'input_handling.c', 'tokenization.c', 'echo.c', 'exit.c',
        'quote_handling.c', 'utilities.c', 'utilities_two.c',
        'commands.c', 'setup_signal_handlers.c', 'main.c', 'cd.c',
        'unset.c', 'commands_extra.c', 'cleaning.c', 'pwd.c',
        'pipes.c', 'env.c', 'export.c', 'redir.c', 'ft_lsttochpp.c',
        'export_helpers.c'
    ]
    
    base_dir = '/home/jbdmc/Documents/minishell'
    fixed_count = 0
    
    for filename in c_files:
        filepath = os.path.join(base_dir, filename)
        if os.path.exists(filepath):
            if fix_file(filepath):
                fixed_count += 1
                print(f"Fixed: {filename}")
            else:
                print(f"No changes: {filename}")
    
    print(f"\nTotal files modified: {fixed_count}")
