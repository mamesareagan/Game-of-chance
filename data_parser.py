import struct
import platform

"""
This module handles the parsing of user data from a binary file.
"""

def get_pointer_size():
    """Check if the system is 32-bit or 64-bit."""
    architecture = platform.architecture()[0]
    if '64bit' in architecture:
        return 8  # 64-bit systems use 8-byte pointers
    return 4  # 32-bit systems use 4-byte pointers

def parse_user_data(file_content, pointer_size):
    """Parse user data from the provided file content."""
    user_struct_size = 12 + 30 + pointer_size  # 12 bytes for 3 integers, 30 bytes for name, and pointer_size

    total_users = len(file_content) // user_struct_size
    offset = 0

    print(f"\nTotal users registered: {total_users}\n")  

    for i in range(total_users):
        user_offset = offset + (i * user_struct_size)

        # Read the user struct: 3 integers, 30-byte name, and function pointer
        uid, credits, highscore = struct.unpack_from('<III', file_content, user_offset)
        name = file_content[user_offset + 12:user_offset + 42].decode('ascii').strip('\x00')  # 30-byte name
        current_game_ptr = struct.unpack_from('<I' if pointer_size == 4 else '<Q', file_content, user_offset + 42)[0]

        # Display the parsed user info
        print(f"User {i + 1}:")
        print(f"  UID: {uid}")
        print(f"  Credits: {credits}")
        print(f"  Highscore: {highscore}")
        print(f"  Name: {name}")
        print(f"  Current Game Pointer: {hex(current_game_ptr)}\n")

def parse_chance_data(file_path):
    """Parse user data from the specified file path."""
    pointer_size = get_pointer_size()
    try:
        with open(file_path, 'rb') as file:
            file_content = file.read()
            parse_user_data(file_content, pointer_size)
    except PermissionError:
        print(f"Error: Permission denied for file {file_path}. Please check the file's permissions.")
    except FileNotFoundError:
        print(f"Error: File {file_path} not found.")

if __name__ == "__main__":
    parse_chance_data('/var/chance.data')
