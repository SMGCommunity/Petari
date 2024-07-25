import csv, datetime, glob, json, io, math, os, sys
from git import Repo
from pathlib import Path
from colorama import Fore, Style
import pandas as pd
import plotly.express as px

# MSL_C++ will not have any progress to generate
LIBRARIES = [ "Game", "JSystem", "MetroTRK", "MSL_C", "nw4r", "Runtime", "RVL_SDK", "RVLFaceLib" ]

def truncate(number, digits) -> float:
    stepper = 10.0 ** digits
    return math.trunc(stepper * number) / stepper

def generateFullProgJSON(lib, label, percent, color):
    json = []
    json.append("{\n")
    json.append("\t\"schemaVersion\": 1,\n")
    json.append(f"\t\"label\": \"{label}\",\n")
    json.append(f"\t\"message\": \"{truncate(percent, 3)}%\",\n")
    json.append(f"\t\"color\": \"{color}\"\n")
    json.append("}")

    with open(f"libs/{lib}/data/{label}.json", "w") as w:
        w.writelines(json)

class Function:
    name = ""
    status = ""
    funcSize = 0

    def __init__(self, name, status, funcSize):
        self.name = name
        self.status = status
        self.funcSize = funcSize

class Object:
    name = ""
    functions = []
    totalFunctions = 0
    totalCompletedFunctions = 0
    totalNonMatchingMinorFunctions = 0
    totalNonMatchingMajorFunctions = 0

    def __init__(self, name):
        self.name = name
        self.functions = list()
        self.totalFunctions = 0
        self.totalCompletedFunctions = 0
        self.totalNonMatchingMinorFunctions = 0
        self.totalNonMatchingMajorFunctions = 0

    def addFunction(self, function):
        self.functions.append(function)

        if function.status == "true":
            self.totalCompletedFunctions += 1
        elif function.status == "minor":
            self.totalNonMatchingMinorFunctions += 1
        elif function.status == "major":
            self.totalNonMatchingMajorFunctions += 1
        
        self.totalFunctions += 1

    def getFunctions(self):
        return self.functions

    def calculateProgress(self):
        fullSize = 0
        matchingSize = 0
        minorSize = 0
        majorSize = 0
        numFuncs = 0
        matchingFuncs = 0
        minorFuncs = 0
        majorFuncs = 0

        for function in self.functions:
            fullSize += function.funcSize
            numFuncs += 1
            
            if function.status == "true":
                matchingSize += function.funcSize
                matchingFuncs += 1
            elif function.status == "minor":
                minorSize += function.funcSize
                minorFuncs += 1
            elif function.status == "major":
                majorSize += function.funcSize
                majorFuncs += 1

        return matchingSize, minorSize, majorSize, fullSize, numFuncs, matchingFuncs, minorFuncs, majorFuncs
        #return doneSize, fullSize, numFuncs, doneFuncs

class Archive:
    parent = ""
    name = ""
    objects = []

    def __init__(self, parent, name):
        self.parent = parent
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
        matchingSize = 0
        minorSize = 0
        majorSize = 0
        numFuncs = 0
        matchingFuncs = 0
        minorFuncs = 0
        majorFuncs = 0

        for obj in self.objects:
            match_size, minor_size, major_size, full_size, num_funcs, matching_funcs, minor_funcs, major_funcs = obj.calculateProgress()
            fullSize += full_size
            matchingSize += match_size
            minorSize += minor_size
            majorSize += major_size
            numFuncs += num_funcs
            matchingFuncs += matching_funcs
            minorFuncs += minor_funcs
            majorFuncs += major_funcs
        
        return matchingSize, minorSize, majorSize, fullSize, numFuncs, matchingFuncs, minorFuncs, majorFuncs


    def getName(self):
        return self.name

    def generateJSONTag(lib, self, percent, color):
        json = []
        json.append("{\n")
        json.append("\t\"schemaVersion\": 1,\n")
        json.append(f"\t\"label\": \"{self.name}\",\n")
        json.append(f"\t\"message\": \"{truncate(percent, 3)}%\",\n")
        json.append(f"\t\"color\": \"{color}\"\n")
        json.append("}")

        with open(f"libs\\{lib}\\data\\json\\{self.name}.json", "w") as w:
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
            matchingSize, minorSize, majorSize, fullSize, numFuncs, matchingFuncs, minorFuncs, majorFuncs = obj.calculateProgress()
            prog = (matchingSize / fullSize) * 100.0
            funcProg = (obj.totalCompletedFunctions / obj.totalFunctions) * 100.0

            marker = ":x:"

            if matchingSize == fullSize:
                marker = ":white_check_mark:"
            elif matchingSize != fullSize and matchingSize != 0:
                marker = ":eight_pointed_black_star:"

            obj_page_name = obj.name.replace(".o", "")

            page.append(f"| [{obj.name}](https://github.com/shibbo/Petari/blob/master/docs/lib/{self.parent}/{self.name}/{obj_page_name}.md) | {prog}% | {obj.totalCompletedFunctions} / {obj.totalFunctions} | {funcProg}% | {marker} \n")

        if not os.path.exists(f"docs\\lib\\{self.parent}"):
            os.makedirs(f"docs\\lib\\{self.parent}")

        with open(f"docs/lib/{self.parent}/{self.name}.md", "w") as w:
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

            matchingSize, minorSize, majorSize, fullSize, numFuncs, matchingFuncs, minorFuncs, majorFuncs = obj.calculateProgress()
            percent = (matchingFuncs / numFuncs) * 100.0

            obj_page.append(f"# {matchingFuncs} / {numFuncs} Completed -- ({percent}%)\n")

            obj_page.append(f"# {obj.name}\n")
            obj_page.append("| Symbol | Decompiled? |\n")
            obj_page.append("| ------------- | ------------- |\n")

            for func in obj.getFunctions():
                marker = ":x:"

                if func.status == "true":
                    marker = ":white_check_mark:"

                obj_page.append(f"| `{func.name}` | {marker} |\n")

            obj_page_name = obj.name.replace(".o", "")

            if not os.path.exists(f"docs\\lib\\{self.parent}\\{self.name}"):
                os.makedirs(f"docs\\lib\\{self.parent}\\{self.name}")

            with open(f"docs\\lib\\{self.parent}\\{self.name}\\{obj_page_name}.md", "w") as w:
                w.writelines(obj_page)

libraries = {}

def doProgress(parent_lib):
    # start by reading function sizes
    func_sizes = {}
    archives = []
    
    with open("data\\funcSizes.txt", "r") as file:
        lines = file.readlines()

        for line in lines:
            spl = line.split('=')
            sym = spl[0]
            func_sizes[sym] = spl[1].split(',', 1)[1]

    if parent_lib != "Game":
        csv_files = glob.glob(f"libs\\{parent_lib}\\csv\\*.csv")
    else:
        csv_files = glob.glob("csv\\*.csv")

    for csv_file in sorted(csv_files, key=str.casefold):
        lib_name = Path(csv_file).stem
        lib_arch_name = Path(csv_file).stem + ".a"

        archive = Archive(parent_lib, lib_name)

        with open(csv_file, "r") as c:
            csv_reader = csv.reader(c)

            for row in csv_reader:
                symbol = row[0]
                symbol = symbol.replace("&#44;", ",")
                if symbol == "Symbol Name":
                    continue

                obj = row[1]
                lib = row[2]
                status = row[3]

                funcSize = int(func_sizes[symbol].strip("\n"))
                func = Function(symbol, status, funcSize)

                obj = Object(obj)
                archive.addFunctionToObject(obj, func)

        archives.append(archive)
    libraries[parent_lib] = archives

for lib in LIBRARIES:
    doProgress(lib)

game_matching_done = 0
game_minor = 0
game_major = 0
game_total = 0

game_funcs_matching = 0
game_funcs_minor = 0
game_funcs_major = 0
game_funcs_total = 0

for key in libraries:
    
    for arch in libraries[key]:
        arch.generateMarkdown()
        matchingSize, minorSize, majorSize, fullSize, numFuncs, matchingFuncs, minorFuncs, majorFuncs = arch.calculateProgress()
        
        # we are really only doing calculations for our main game
        if key == "Game":
            game_matching_done += matchingSize
            game_minor += minorSize
            game_major += majorSize
            game_total += fullSize
            game_funcs_matching += matchingFuncs
            game_funcs_minor += minorFuncs
            game_funcs_major += majorFuncs
            game_funcs_total += numFuncs

# printing game specific stuff
prog = (game_matching_done / game_total) * 100.0
prog_minor = (game_minor / game_total) * 100.0
prog_major = (game_major / game_total) * 100.0
prog_total = prog + prog_minor + prog_major
total_size = game_matching_done + game_funcs_minor + game_funcs_major
func_prog = (game_funcs_matching / game_funcs_total)
print(f"Functions: {truncate(func_prog, 4)}% [{game_funcs_matching} / {game_funcs_total}]")
print(f"{Fore.BLUE}decompiled:{Style.RESET_ALL} {truncate(prog_total, 4)}% [{total_size} / {game_total}]")
print(f"{Fore.GREEN}matching:{Style.RESET_ALL} {truncate(prog, 4)}% [{game_funcs_matching} / {game_total}]")
print(f"{Fore.YELLOW}non-matching (minor):{Style.RESET_ALL} {truncate(prog_minor, 4)}% [{game_funcs_minor} / {game_total}]")
print(f"{Fore.RED}non-matching (major):{Style.RESET_ALL} {truncate(prog_major, 4)}% [{game_funcs_major} / {game_total}]")