import re
import os
from urllib import request
import requests
import shutil

from_path = r'D:\Users\vmp\Desktop\weharmony.github.io-master\blog'

def download_images(path):
    with open(path, 'r', encoding='utf-8') as f:
        content = f.read()
        matches = re.findall(r"!\[.*\]\((.+)\)", content)
        for url in matches:
            if url.startswith("http") or url.startswith("https"):
                print("origin :", url)
                print("path   :", path)
                orig_path = url.split("resources/")
                if len(orig_path) > 1:
                    dummy = orig_path[1].replace("/", "\\")
                    print("dirname:", dummy)
                    asset_path = os.path.join(
                        os.path.dirname(path), 'assets', dummy)
                    print("output :", asset_path)
                    if not os.path.exists(os.path.dirname(asset_path)):
                        os.makedirs(os.path.dirname(asset_path))
                    if not os.path.exists(asset_path):
                        with requests.get(url, stream=True) as r, open(asset_path, 'wb') as f:
                            shutil.copyfileobj(r.raw, f)
                    content = content.replace(
                        url, f"./assets/{orig_path[1]}")

        with open(path, 'w', encoding='utf-8') as f:
            f.write(content)

def pic_fetch(file_path):
    # Replace url
    with open(file_path, 'r', encoding='utf-8') as rf:
        line_list = []
        for line in rf:
            match_list = re.findall(r'!\[.*\]\((.+)\)', line)
            if match_list:
                for origin_url in match_list:
                    print("origin :", origin_url)
                    m = re.findall(r'https:.*\.\.(.*)', origin_url)
                    if len(m):
                        output_file = os.path.dirname(file_path) + m[0]
                        print("Pic path", output_file)
                        output_dir = os.path.dirname(output_file)
                        print("Output pic: ", output_dir)
                        if not os.path.exists(output_dir):
                            os.makedirs(output_dir)
                        request.urlretrieve(origin_url, output_file)
                        output_url = "." + m[0]
                        line = line.replace(origin_url, output_url)
            line_list.append(line)

        # Write file
        content = ""
        with open(file_path, 'w', encoding='utf-8') as wf:
            for line in line_list:
                content += line
            if content:
                wf.write(content)
                print("\n" + file_path + "\n===========================\n")
def get_filelist(dir, Filelist):
    newDir = dir
    if os.path.isfile(dir) and dir.endswith("md"):
        Filelist.append(dir)
        # # 若只是要返回文件文，使用这个
        # Filelist.append(os.path.basename(dir))
    elif os.path.isdir(dir):
        for s in os.listdir(dir):
            # 如果需要忽略某些文件夹，使用以下代码
            #if s == "xxx":
                #continue
            newDir=os.path.join(dir,s)
            get_filelist(newDir, Filelist)
    return Filelist

if __name__ == '__main__':
    items = get_filelist(from_path, [])
    for item in items:
        print("file: ", item)
        download_images(item)
