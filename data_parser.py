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
    user_struct_size = 12 + 100 + pointer_size  # 12 bytes for 3 integers, 100 bytes for name, and pointer_size

    offset = 0

    # Read the original first 12 bytes (3 integers)
    int1, int2, int3 = struct.unpack_from('<III', file_content, offset)
    offset += 12  # Move offset by 12 bytes

    # Read the name (100 bytes)
    name = file_content[offset:offset + 100].decode('ascii').strip('\x00')  # Decode and strip nulls
    offset += 100  # Move the offset forward by 100 bytes

    print(f"Integer 1: {int1}")
    print(f"Integer 2: {int2}")
    print(f"Integer 3: {int3}")
    print(f"Name: {name}")

    # Now read user structs
    total_users = (len(file_content) - offset) // user_struct_size

    print(f"\nTotal users registered: {total_users + 1}\n")  # +1 for the first user

    print("User 1:")
    print(f"  UID: {int1}")  # Assuming int1 is the UID for the first user
    print(f"  Credits: {int2}")  # Assuming int2 is the Credits
    print(f"  Highscore: {int3}")  # Assuming int3 is the Highscore
    print(f"  Name: {name}")
    print(f"  Current Game Pointer: N/A\n")  # No pointer for the first user

    for i in range(total_users):
        user_offset = offset + (i * user_struct_size)

        # Read the user struct: 3 integers, 100-byte name, and function pointer (int/long based on system)
        uid, credits, highscore = struct.unpack_from('<III', file_content, user_offset)
        name = file_content[user_offset + 12:user_offset + 112].decode('ascii').strip('\x00')  # Decode and strip nulls
        current_game_ptr = struct.unpack_from('<I' if pointer_size == 4 else '<Q', file_content, user_offset + 112)[0]

        # Display the parsed user info
        print(f"User {i + 2}:")  # Start counting from 2 since the first user is already printed
        print(f"  UID: {uid}")
        print(f"  Credits: {credits}")
        print(f"  Highscore: {highscore}")
        print(f"  Name: {name}")
        print(f"  Current Game Pointer: {current_game_ptr}\n")

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

