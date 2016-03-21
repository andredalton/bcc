def main():
    nums = map(int, raw_input().split())
    n = nums[0]
    del nums[0]
    nums = set(nums)
    nums = list(nums)
    nums.sort()
    if n%nums[0]==0:
        print n/nums[0]
    else:
        for i in range(len(nums)-1, -1, -1):
            if nums[i] > n:
                del nums[i]
            else:
                break
        if len(nums) == 3 and nums[2]%nums[0] == 0:
            del nums[2]
        if len(nums) > 1 and nums[1]%nums[0] == 0:
            del nums[1]
        if len(nums) == 3 and nums[2]%(nums[0]+nums[1]) == 0:
            del nums[2]
        if len(nums) == 1:
            print n/nums[0]
        elif len(nums) == 2:
            for i in range(1+n/nums[1]):
                if (n - i * nums[1]) % nums[0] == 0:
                    break
            print i + (n-i*nums[1])/nums[0]
        else:
            maximo = 0
            for i in range(1+n/nums[2]):
                for j in range(1+n/nums[1]):
                    if n < i*nums[2] + j*nums[1]:
                        break
                    if (n-i*nums[2]-j*nums[1]) % nums[0] == 0:
                        if i+j+(n-i*nums[2]-j*nums[1])/nums[0] > maximo:
                            maximo = i+j+(n-i*nums[2]-j*nums[1])/nums[0]
            print maximo


if __name__ == '__main__':
    main()