import struct
import platform

def get_pointer_size():
    """Check if the system is 32-bit or 64-bit."""
    architecture = platform.architecture()[0]
    if '64bit' in architecture:
        return 8  # 64-bit systems use 8-byte pointers
    else:
        return 4  # 32-bit systems use 4-byte pointers

def parse_user_data(file_content, pointer_size):
    # Define the base user structure size
    USER_STRUCT_SIZE = 12 + 100 + pointer_size  # 12 bytes for 3 integers, 100 bytes for name, and pointer_size

    offset = 0

    # Read the original first 12 bytes (3 integers)
    int1, int2, int3 = struct.unpack_from('<III', file_content, offset)
    offset += 12  # Move offset by 12 bytes

    # Read the next  bytes for the string 
    name = file_content[offset:offset + 30].decode('ascii').strip('\x00')  # Decode and strip nulls
    offset += 3

    # Now read user structs
    total_users = (len(file_content) - offset) // USER_STRUCT_SIZE

    # Include the first user directly after parsing the first 12 bytes
    print(f"\nTotal users registered: {total_users + 1}\n")  # +1 for the first user

    # Print the first user directly
    print("User 1:")
    print(f"  UID: {int1}")  # Assuming int1 is the UID for the first user
    print(f"  Credits: {int2}")  # Assuming int2 is the Credits
    print(f"  Highscore: {int3}")  # Assuming int3 is the Highscore
    print(f"  Name: {name}")
    print(f"  Current Game Pointer: N/A\n")  # No pointer for the first user

    # Start reading from the next user
    for i in range(total_users):
        user_offset = offset + (i * USER_STRUCT_SIZE)
        
        # Read the user struct: 3 integers, 100-byte name, and function pointer (int/long based on system)
        uid, credits, highscore = struct.unpack_from('<III', file_content, user_offset)
        name = file_content[user_offset + 12:user_offset + 112].decode('ascii').strip('\x00')  # Decode and strip nulls
        if pointer_size == 4:
            current_game_ptr = struct.unpack_from('<I', file_content, user_offset + 112)[0]  # 32-bit pointer
        else:
            current_game_ptr = struct.unpack_from('<Q', file_content, user_offset + 112)[0]  # 64-bit pointer
        
        # Display the parsed user info
        print(f"User {i + 2}:")  # Start counting from 2 since the first user is already printed
        print(f"  UID: {uid}")
        print(f"  Credits: {credits}")
        print(f"  Highscore: {highscore}")
        print(f"  Name: {name}")
        print(f"  Current Game Pointer: {current_game_ptr}\n")

def parse_chance_data(file_path):
    # Determine pointer size based on system architecture (32-bit vs 64-bit)
    pointer_size = get_pointer_size()
    try:
        with open(file_path, 'rb') as file:
            # Read the entire file into memory
            file_content = file.read()

            # Parse the data with the correct pointer size
            parse_user_data(file_content, pointer_size)
    except PermissionError:
        print(f"Error: Permission denied for file {file_path}. Please check the file's permissions.")
    except FileNotFoundError:
        print(f"Error: File {file_path} not found.")

if __name__ == "__main__":
    parse_chance_data('/var/chance.data')
