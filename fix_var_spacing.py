#!/usr/bin/env python3
"""
Fix variable declaration spacing - handle multiple spaces between type and name
"""
import re
import os

def fix_var_declarations(content):
    """Fix variable declarations with multiple spaces"""
    lines = content.split('\n')
    fixed = []
    
    for line in lines:
        # Pattern: type followed by multiple spaces then variable name
        # Handle cases like "size_t  i;" or "char    *res;"
        # Replace with single tab between type and variable name
        
        # Skip empty lines and comment lines
        if not line.strip() or line.strip().startswith('//') or line.strip().startswith('/*'):
            fixed.append(line)
            continue
        
        # Skip lines in comments (multiline comments)
        if line.strip().startswith('*'):
            fixed.append(line)
            continue
        
        # Match variable declarations: type + multiple spaces + name (with optional pointer/bracket)
        # Pattern: (^\s*)([a-zA-Z_][a-zA-Z0-9_]*(\s+\*)?)(  +)([a-zA-Z_*])
        # This matches type, multiple spaces, and variable name
        
        match = re.match(r'^(\t*)([a-zA-Z_][a-zA-Z0-9_]*(\s+\*)?)(  +)([a-zA-Z_*][a-zA-Z0-9_\[\]*]*.*?)$', line)
        if match:
            indent = match.group(1)
            type_part = match.group(2)
            var_rest = match.group(5)
            # Replace multiple spaces with single tab
            new_line = indent + type_part + '\t' + var_rest
            fixed.append(new_line)
        else:
            fixed.append(line)
    
    return '\n'.join(fixed)

def process_files():
    """Process all C files in the directory"""
    base_dir = '/home/jbdmc/Documents/minishell'
    
    c_files = [f for f in os.listdir(base_dir) 
               if f.endswith('.c') or f.endswith('.h')]
    
    for filename in sorted(c_files):
        if filename.endswith('.h') and filename != 'minishell.h':
            continue
            
        filepath = os.path.join(base_dir, filename)
        
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            
            fixed = fix_var_declarations(content)
            
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
