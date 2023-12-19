import os
from subprocess import Popen, PIPE

outDirPrefix = "decompiled"

inFolder = os.path.join(".", "os")
decompiler = os.path.join(".", "sofos.exe")

counter = 0
error_counter = 0
success_counter = 0

# Iterate the os directory.
for root, dirs, files in os.walk(inFolder):
	for f in files:
		# tail = top-most directory.
		itDirTail = os.path.split(root)[1]
		# norm removes redundant ./ from path
		# print(root)
		# print(os.path.join(root,f))
		itFilePath = os.path.normpath(os.path.join(root,f))
		
		outDirPath = os.path.join(outDirPrefix,itDirTail)
		try:
			os.makedirs(outDirPath)
			# print(f"Created directory : {outDirPath}")
		except:
			pass

		outFilepath = os.path.join(outDirPrefix,itDirTail,f)
		if os.path.splitext(outFilepath)[1].lower() != ".os":
			continue
		outFilepath = os.path.splitext(outFilepath)[0] + ".ds"


		file_in = itFilePath
		file_out = outFilepath

		counter+=1
		# print('DEBUG: ' + decompiler + ' ' + file_in +  ' ' + file_out)

		# Using 'with' to ensure proper cleanup
		with Popen([decompiler, file_in, file_out], stdin=PIPE, stdout=PIPE, stderr=PIPE, shell=False) as p:
			# Using 'communicate' to capture output and error
			output, err = p.communicate()

			# Decoding byte strings to Unicode
			output = output.decode('utf-8')
			err = err.decode('utf-8')

			try:
				# Detecting Errors only if the file exists
				out_filesize = os.stat(file_out).st_size
				# print(f"{file_out}:{out_filesize}")
			except FileNotFoundError:
				# Handle the case where the file does not exist
				print(f"Error: Output file '{file_out}' not found.")

			# print(f"{file_out}:{out_filesize}")

			# Check for errors
			if p.returncode != 0 or out_filesize == 0 or err:
				error_counter += 1
				print("Error occurred:")
				print(f"Return Code: {p.returncode}")
				print(f"Error Output: {err}")
				print(f"stdout Output: {output}")
				print()
				print()
			else:
				success_counter += 1
				print("Decompilation successful!")

print(f"Completed batch run\nErrors : {error_counter}/{counter}\nCompleted: {success_counter}/{counter}")