p, r = map(int, input("Enter the no. of processes and resources: ").split())

print("Enter the claim matrix:")
claim = [list(map(int, input().split())) for i in range(p)]

print("Enter the allocation matrix:")
alloc = [list(map(int, input().split())) for i in range(p)]

total = list(map(int, input("Resource vector:").split()))

# Calculate available
avail = []
for j in range(r):
    s = 0
    for i in range(p):
        s += alloc[i][j]
    avail.append(total[j] - s)

# Calculate need and find safe sequence
work = avail.copy()
finish = [False] * p

while not all(finish):
    found = False
    for i in range(p):
        if not finish[i]:
            need = [claim[i][j] - alloc[i][j] for j in range(r)]
            if all(need[j] <= work[j] for j in range(r)):
                print(f"All the resources can be allocated to Process {i+1}")
                for j in range(r):
                    work[j] += alloc[i][j]
                
                # Fix: Dynamically print available resources based on number of resources
                available_str = " ".join(str(work[j]) for j in range(r))
                print(f"Available resources are: {available_str}")
                
                print(f"Process {i+1} executed?: y")
                finish[i] = True
                found = True
                break  # Break to restart the loop from beginning
    
    if not found:
        break

print()
if all(finish):
    print("System is in safe mode")
    print("The given state is safe state")
else:
    print("System is not in safety state")