from termcolor import cprint 
from pyfiglet import figlet_format
import os
import filetype
import hashlib
import exiftool

# Start of the Script

cprint(figlet_format('Folder Cleaner', font='starwars'), attrs=['bold'])
print("[+] Starting to clean folders...")
parent_path = os.getcwd();

# taking a list of all files in the current dir

files = []
folder_names = set()
for path in os.listdir(parent_path):
    if os.path.isfile(os.path.join(parent_path, path)):
        files.append(path)

def get_category(file_extension):
    print(f"file ext : {file_extension}")
    with exiftool.ExifToolHelper() as et:
        try : 
            metadata = et.get_metadata(file_extension, params=['-m'])
            return metadata[0]["File:MIMEType"].split('/')[0]
        except exiftool.exceptions.ExifToolExecuteError as e:
            print(f"ExifTool error: {e}")
            print("STDOUT:", e.stdout)
            print("STDERR:", e.stderr)

# organising files
print(files)
for file in files :
    kind = filetype.guess(file)
    if kind is None:
        filetype_str = file.split(".")[-1]
    else : 
        filefiletype_strtype = kind.extension
    print(file)
    category = get_category(os.getcwd() + "/" + file)
    folder_names.add(category)
    
    # create folder for the category is the category is not present

    folder_creating_path = parent_path + "/" + category
    if not os.path.isdir(folder_creating_path) : 
        print(f"[+] Creating {category} folder...")
        os.mkdir(folder_creating_path)
    
    print(f"[+] Moving {file} to {category} folder...")
    os.replace(os.getcwd() + "/" + file, os.getcwd() + "/" + category + "/" + file)

# deleting duplicate files code

print("[+] Deleting duplicate files in folders")

def get_file_hash(file_path, hash_algo='sha256'):
    hash_function = hashlib.new(hash_algo)
    with open(file_path, 'rb') as file:
        while chunk := file.read(8192):  # Read in 8k chunks to handle large files
            hash_function.update(chunk)
    return hash_function.hexdigest()

def delete_duplicate_files(folder_path, hash_algo='sha256'):
    seen_hashes = set()  
    files_to_delete = [] 
    
    for root, _, files in os.walk(folder_path):
        for file_name in files:
            file_path = os.path.join(root, file_name)
            try:
                file_hash = get_file_hash(file_path, hash_algo)
                if file_hash in seen_hashes:
                    files_to_delete.append(file_path)
                else:
                    seen_hashes.add(file_hash)
                    
            except Exception as e:
                print(f"- Error reading file {file_path}: {e}")
    
    for file_path in files_to_delete:
        try:
            os.remove(file_path)
            print(f"- Deleted duplicate file: {file_path}")
        except Exception as e:
            print(f"- Error deleting file {file_path}: {e}")

for folder_name in folder_names :
    folder_creating_path = parent_path + "/" + folder_name
    delete_duplicate_files(folder_creating_path)

# final print

print("[+] Done With moving files")
