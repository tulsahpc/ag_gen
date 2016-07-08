#!/usr/bin/env ruby

$l = 3
$cur = 0
$arr = [0, 0, 0]

def can_inc idx
    if $arr[idx] and $arr[idx] < $l
        true
    else
        false
    end
end

def inc idx
    $arr[idx] += 1 if $arr[idx]
end

def reset idx
    for i in 0...idx
        $arr[i] = 0
    end
    $cur = 0
end

while $cur < $l
    p $arr
    if can_inc $cur
        inc $cur
    else
        $cur += 1 while $arr[$cur] and !can_inc($cur)
        if $arr[$cur]
            inc $cur
            reset $cur
        end
    end
end
