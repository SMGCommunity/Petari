import csv, datetime, glob, json, io, math, os, sys
from git import Repo
from pathlib import Path

import pandas as pd
import plotly.express as px

libraries = { }

def truncate(number, digits) -> float:
    stepper = 10.0 ** digits
    return math.trunc(stepper * number) / stepper

def generateFullProgJSON(label, percent, color):
    json = []
    json.append("{\n")
    json.append("\t\"schemaVersion\": 1,\n")
    json.append(f"\t\"label\": \"{label}\",\n")
    json.append(f"\t\"message\": \"{truncate(percent, 3)}%\",\n")
    json.append(f"\t\"color\": \"{color}\"\n")
    json.append("}")

    with open(f"data/{label}.json", "w") as w:
        w.writelines(json)

class Function:
    name = ""
    isCompleted = False
    funcSize = 0

    def __init__(self, name, isComplete, funcSize):
        self.name = name
        self.isCompleted = isComplete
        self.funcSize = funcSize

class Object:
    name = ""
    functions = []
    totalFunctions = 0
    totalCompletedFunctions = 0

    def __init__(self, name):
        self.name = name
        self.functions = list()
        self.totalFunctions = 0
        self.totalCompletedFunctions = 0

    def addFunction(self, function):
        self.functions.append(function)

        if function.isCompleted:
            self.totalCompletedFunctions += 1
        
        self.totalFunctions += 1

    def getFunctions(self):
        return self.functions

    def calculateProgress(self):
        fullSize = 0
        doneSize = 0
        numFuncs = 0
        doneFuncs = 0
        for function in self.functions:
            fullSize += function.funcSize
            numFuncs += 1
            
            if function.isCompleted:
                doneSize += function.funcSize
                doneFuncs += 1

        return doneSize, fullSize, numFuncs, doneFuncs

class Library:
    name = ""
    objects = []

    def __init__(self, name):
        self.name = name
        self.objects = list()

    def addObject(self, object):
        self.objects.append(object)

    def addFunctionToObject(self, obj, function):
        if self.containsObject(obj.name):
            self.findObject(obj.name).addFunction(function)
        else:
            self.addObject(obj)
            self.addFunctionToObject(obj, function)

    def findObject(self, objectName):
        for obj in self.objects:
            if obj.name == objectName:
                return obj

        return None

    def getObjects(self):
        return self.objects
    
    def containsObject(self, object):
        for obj in self.objects:
            if obj.name == object:
                return True

        return False

    def calculateProgress(self):
        fullSize = 0
        doneSize = 0
        funcNum = 0
        funcDone = 0

        for obj in self.objects:
            d, f, func_num, func_done = obj.calculateProgress()
            fullSize += f
            doneSize += d
            funcNum += func_num
            funcDone += func_done
        
        return doneSize, fullSize, funcNum, funcDone


    def getName(self):
        return self.name

    def generateJSONTag(self, percent, color):
        json = []
        json.append("{\n")
        json.append("\t\"schemaVersion\": 1,\n")
        json.append(f"\t\"label\": \"{self.name}\",\n")
        json.append(f"\t\"message\": \"{truncate(percent, 3)}%\",\n")
        json.append(f"\t\"color\": \"{color}\"\n")
        json.append("}")

        with open(f"data/json/{self.name}.json", "w") as w:
            w.writelines(json)

    def generateMarkdown(self):
        # first we are going to generate the tables for the object files themselves in the library
        page = []
        page.append(f"# {self.name}\n")
        page.append("| Symbol | Meaning \n")
        page.append("| ------------- | ------------- \n")
        page.append("| :x: | Object has not yet been started. \n")
        page.append("| :eight_pointed_black_star: | Object is in progress. \n")
        page.append("| :white_check_mark: | Object is completed. \n")
        page.append("\n\n")
        page.append("| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) | Status \n")
        page.append("| ------------- | ------------- | ------------- | ------------- | ------------- \n")

        for obj in self.objects:
            d, f, pad1, pad2 = obj.calculateProgress()
            prog = (d / f) * 100.0
            funcProg = (obj.totalCompletedFunctions / obj.totalFunctions) * 100.0

            marker = ":x:"

            if d == f:
                marker = ":white_check_mark:"
            elif d != f and d != 0:
                marker = ":eight_pointed_black_star:"

            obj_page_name = obj.name.replace(".o", "")

            page.append(f"| [{obj.name}](https://github.com/shibbo/Petari/blob/master/docs/lib/{self.name}/{obj_page_name}.md) | {prog}% | {obj.totalCompletedFunctions} / {obj.totalFunctions} | {funcProg}% | {marker} \n")

        with open(f"docs/lib/{self.name}.md", "w") as w:
            w.writelines(page)

        # now that we have written the main page, let's make the object page too
        for obj in self.objects:
            obj_page = []

            obj_page.append(f"# {obj.name}\n")
            obj_page.append("| Symbol | Meaning \n")
            obj_page.append("| ------------- | ------------- \n")
            obj_page.append("| :x: | Function has not yet been started or is not matching. \n")
            obj_page.append("| :white_check_mark: | Function is completed. \n")
            obj_page.append("\n\n")

            pad1, pad2, numFunc, doneFunc = obj.calculateProgress()
            percent = (doneFunc / numFunc) * 100.0

            obj_page.append(f"# {doneFunc} / {numFunc} Completed -- ({percent}%)\n")

            obj_page.append(f"# {obj.name}\n")
            obj_page.append("| Symbol | Decompiled? |\n")
            obj_page.append("| ------------- | ------------- |\n")

            for func in obj.getFunctions():
                marker = ":x:"

                if func.isCompleted:
                    marker = ":white_check_mark:"

                obj_page.append(f"| `{func.name}` | {marker} |\n")

            obj_page_name = obj.name.replace(".o", "")

            if not os.path.exists(f"docs/lib/{self.name}"):
                os.makedirs(f"docs/lib/{self.name}")

            with open(f"docs/lib/{self.name}/{obj_page_name}.md", "w") as w:
                w.writelines(obj_page)

game_libs = [
    "Animation.a",
    "AreaObj.a",
    "AudioLib.a",
    "Boss.a",
    "Camera.a",
    "Demo.a",
    "Effect.a",
    "Enemy.a",
    "GameAudio.a",
    "Gravity.a",
    "LiveActor.a",
    "Map.a",
    "MapObj.a",
    "NameObj.a",
    "NPC.a",
    "NWC24.a",
    "Player.a",
    "RhythmLib.a",
    "Ride.a",
    "Scene.a",
    "Screen.a",
    "Speaker.a",
    "System.a",
    "Util.a"
]

lib_percent_colors = {
    "Animation": "brightgreen",
    "AreaObj": "green",
    "AudioLib": "yellow",
    "Boss": "orange",
    "Camera": "red",
    "Demo": "D65076",
    "Effect": "pink",
    "Enemy": "magenta",
    "GameAudio": "teal",
    "Gravity": "maroon",
    "LiveActor": "cyan",
    "Map": "silver",
    "MapObj": "tan",
    "NameObj": "indigo",
    "NPC": "7fffd4",
    "Player": "ff7f50",
    "RhythmLib": "088da5",
    "Ride": "ffff66",
    "Scene": "a0db8e",
    "Screen": "ff4040",
    "Speaker": "daa520",
    "System": "696969",
    "Util": "ff6666"
}

func_sizes = {}

# start by reading function sizes
with open("data/funcSizes.txt", "r") as file:
    lines = file.readlines()

    for line in lines:
        spl = line.split('=')
        sym = spl[0]
        func_sizes[sym] = spl[1].split(',', 1)[1]

csv_files = glob.glob("csv/*.csv")

for csv_file in sorted(csv_files, key=str.casefold):
    lib_name = Path(csv_file).stem
    lib_arch_name = Path(csv_file).stem + ".a"

    library = Library(lib_name)

    with open(csv_file, "r") as c:
        csv_reader = csv.reader(c)

        for row in csv_reader:
            symbol = row[0]
            symbol = symbol.replace("&#44;", ",")
            if symbol == "Symbol Name":
                continue

            obj = row[1]
            lib = row[2]
            done = row[3] == "true"

            funcSize = int(func_sizes[symbol].strip("\n"))
            func = Function(symbol, done, funcSize)

            obj = Object(obj)
            library.addFunctionToObject(obj, func)

    libraries[lib_name] = library

full_sdk_size = 0
done_sdk_size = 0

num_funcs = 0
num_done_funcs = 0

print("Calculating percentages...")

for key in libraries:
    lib = libraries[key]
    d, f, funcNum, funcDoneNum = lib.calculateProgress()

    libName = f"{lib.getName()}.a"

    if libName in game_libs:
        full_sdk_size += f
        done_sdk_size += d
        num_funcs += funcNum
        num_done_funcs += funcDoneNum

    if lib.getName() not in lib_percent_colors:
        lib.generateJSONTag((d / f ) * 100.0, "ffff66")
    else:
        lib.generateJSONTag((d / f ) * 100.0, lib_percent_colors[lib.getName()])

progPercent_sdk = (done_sdk_size / full_sdk_size ) * 100.0
progPercent_Func = (num_done_funcs / num_funcs) * 100.0

print(f"Progress: {progPercent_sdk}% [{done_sdk_size} / {full_sdk_size}] bytes")
print(f"Progress: {progPercent_Func}% [{num_done_funcs} / {num_funcs}] functions")
print("Generating JSON...")

generateFullProgJSON("Game", progPercent_sdk, "blue")

print("Generating markdown pages...")

# now we generate our progress page
progressPage = []
progressPage.append("| Library | Percentage |\n")
progressPage.append("| ------------- | ------------- |\n")

for key in libraries:
    lib = libraries[key]
    d, f, pad1, pad2 = lib.calculateProgress()
    libprog = (d / f) * 100.0
    progressPage.append(f"| [{key}](https://github.com/shibbo/Petari/blob/master/docs/lib/{key}.md) | {libprog}% |\n")

with open("docs/PROGRESS.md", "w") as w:
    w.writelines(progressPage)

# now we write the progress page for each library
for key in libraries:
    lib = libraries[key]
    lib.generateMarkdown()

print("Generating progress graph...")

# now we do the cool progress drawing chart
x_axis = [datetime.datetime.now()]
y_axis = [progPercent_sdk]

# np.seterr(all="ignore")

repo = Repo(".")

for commit in repo.iter_commits(rev='92c1a7e..master'):
    cur_file = None

    try:
        cur_file = commit.tree / 'data' / 'percent.json'
    except:
        try:
            cur_file = commit.tree / 'data' / 'game.json'
        except:
            try:
                cur_file = commit.tree / 'data' / 'Game.json'
            except:
                pass
            pass
        pass

    if cur_file is None:
        continue

    with io.BytesIO(cur_file.data_stream.read()) as f:
        try:
            percent_str = json.loads(f.read().decode('utf-8'))['message'].strip("%")
            x_axis.append(datetime.datetime.fromtimestamp(commit.committed_date))
            y_axis.append(float(percent_str))
        except:
            continue

df = pd.DataFrame({'date': x_axis, 'progress': y_axis})
fig = px.line(df, x='date', y='progress', title='Petari Progress', line_shape='hv', markers=False)
fig.update_yaxes(ticksuffix='%')
fig.write_image('prog.png')

print("Done.")
