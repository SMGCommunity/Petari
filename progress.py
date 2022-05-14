import csv, glob, math, os, sys
from pathlib import Path

libraries = { }

def truncate(number, digits) -> float:
    stepper = 10.0 ** digits
    return math.trunc(stepper * number) / stepper

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
        for function in self.functions:
            fullSize += function.funcSize
            
            if function.isCompleted:
                doneSize += function.funcSize

        return doneSize, fullSize

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

        for obj in self.objects:
            d, f = obj.calculateProgress()
            fullSize += f
            doneSize += d
        
        return doneSize, fullSize

    def getName(self):
        return self.name

    def generateJSONTag(self, percent, color):
        json = []
        json.append("{\n")
        json.append("\t\"schemaVersion\": 1,\n")
        json.append(f"\t\"label\": \"{self.name}\",\n")
        json.append(f"\t\"message\": \"{truncate(percent, 5)}%\",\n")
        json.append(f"\t\"color\": \"{color}\"\n")
        json.append("}")

        with open(f"data/json/{self.name}.json", "w") as w:
            w.writelines(json)

    def generateMarkdown(self):
        # first we are going to generate the tables for the object files themselves in the library
        page = []
        page.append(f"# {self.name}\n")
        page.append("| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) \n")
        page.append("| ------------- | ------------- | ------------- | ------------- |\n")

        for obj in self.objects:
            d, f = obj.calculateProgress()
            prog = (d / f) * 100.0
            funcProg = (obj.totalCompletedFunctions / obj.totalFunctions) * 100.0
            page.append(f"| {obj.name} | {prog}% | {obj.totalCompletedFunctions} / {obj.totalFunctions} | {funcProg}% |\n")

        page.append("\n\n")

        # now we can do it per object in the library
        for obj in self.objects:
            page.append(f"# {obj.name}\n")
            page.append("| Symbol | Decompiled? |\n")
            page.append("| ------------- | ------------- |\n")

            for func in obj.getFunctions():
                marker = ":x:"

                if func.isCompleted:
                    marker = ":white_check_mark:"

                funcName = func.name.replace("<", "&lt;")
                funcName = funcName.replace(">", "&gt;")

                page.append(f"| {funcName} | {marker} |\n")

            page.append("\n\n")

        with open(f"docs/lib/{self.name}.md", "w") as w:
            w.writelines(page)

excludedLibraries = [
            "ai.a",
            "aralt.a",
            "arc.a",
            "ax.a",
            "axfx.a",
            "base.a",
            "bte.a",
            "db.a",
            "dsp.a",
            "dvd.a",
            "esp.a",
            "euart.a",
            "exi.a",
            "fs.a",
            "gd.a",
            "gx.a",
            "ipc.a",
            "libnw4Fr_ut.a",
            "libnw4r_db.a",
            "libnw4r_lyt.a",
            "libnw4r_math.a",
            "libnw4r_ut.a",
            "MSL_C.PPCEABI.bare.H.a",
            "mem.a",
            "mtx.a",
            "nand.a",
            "net.a",
            "nwc24.a",
            "NWC24.a",
            "os.a",
            "pad.a",
            "rso.a",
            "Runtime.PPCEABI.H.a",
            "RVLFaceLib.a",
            "sc.a",
            "si.a",
            "thp.a",
            "tpl.a",
            "TRK_Hollywood_Revolution.a",
            "usb.a",
            "vf.a",
            "vi.a",
            "wenc.a",
            "wpad.a",
            "wud.a",
            "JAudio2.a",
            "JKernel.a",
            "JSupport.a",
            "JGadget.a",
            "JUtility.a",
            "J2DGraph.a",
            "J3DGraphBase.a",
            "J3DGraphAnimator.a",
            "J3DGraphLoader.a",
            "JMath.a",
            "JParticle.a",
            "NdevExi2A.a"
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

for csv_file in csv_files:
    lib_name = Path(csv_file).stem
    lib_arch_name = Path(csv_file).stem + ".a"
    # we are just going to ignore non-SMG libraries
    if lib_arch_name not in excludedLibraries:
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

fullSize = 0
doneSize = 0

print("Calculating percentages...")

for key in libraries:
    lib = libraries[key]
    d, f = lib.calculateProgress()
    fullSize += f
    doneSize += d
    lib.generateJSONTag((d / f ) * 100.0, lib_percent_colors[lib.getName()])

progPercent = (doneSize / fullSize ) * 100.0
progNonPercent = int((doneSize / fullSize) * 120.0)

print(f"Progress: {progPercent}% [{doneSize} / {fullSize}] bytes")
print(f"You currently have {progNonPercent} / 120 stars.")

print("Generating JSON...")

json = []
json.append("{\n")
json.append("\t\"schemaVersion\": 1,\n")
json.append("\t\"label\": \"decompiled\",\n")
json.append(f"\t\"message\": \"{progPercent}%\",\n")
json.append("\t\"color\": \"blue\"\n")
json.append("}")

with open("data/percent.json", "w") as w:
    w.writelines(json)

print("Generating markdown pages...")

# now we generate our progress page
progressPage = []
progressPage.append("| Library | Percentage |\n")
progressPage.append("| ------------- | ------------- |\n")

for key in libraries:
    lib = libraries[key]
    d, f = lib.calculateProgress()
    libprog = (d / f) * 100.0
    progressPage.append(f"| [{key}](https://github.com/shibbo/Petari/blob/master/docs/lib/{key}.md) | {libprog}% |\n")

with open("docs/PROGRESS.md", "w") as w:
    w.writelines(progressPage)

# now we write the progress page for each library
for key in libraries:
    lib = libraries[key]
    lib.generateMarkdown()

print("Done.")