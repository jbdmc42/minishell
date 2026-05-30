#!/usr/bin/env python3
"""
Remove spaces from empty lines (SPACE_EMPTY_LINE errors)
"""
import os
import re

def fix_empty_lines(content):
    """Remove spaces from empty lines"""
    lines = content.split('\n')
    fixed = []
    
    for line in lines:
        # If line is empty or contains only whitespace
        if not line.strip():
            fixed.append('')  # Replace with completely empty line
        else:
            fixed.append(line)
    
    return '\n'.join(fixed)

def process_files():
    """Process all C files"""
    base_dir = '/home/jbdmc/Documents/minishell'
    
    c_files = [f for f in os.listdir(base_dir) if f.endswith('.c')]
    
    for filename in sorted(c_files):
        filepath = os.path.join(base_dir, filename)
        
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            
            fixed = fix_empty_lines(content)
            
            if fixed != content:
                with open(filepath, 'w', encoding='utf-8') as f:
                    f.write(fixed)
                print(f"Fixed: {filename}")
            else:
                print(f"No changes: {filename}")
                
        except Exception as e:
            print(f"Error with {filename}: {e}")

if __name__ == '__main__':
    process_files()
