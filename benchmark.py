#!/usr/bin/env python3
import subprocess
import time
import redis
import json
from collections import defaultdict

# bench our learned db
def bench_learndb(num_ops=1000):
    results = {}

    # generate commands
    set_cmds = []
    get_cmds = []
    mixed_cmds = []

    for i in range(num_ops):
        set_cmds.append(f"SET key_{i} value_{i}\n")
        get_cmds.append(f"GET key_{i}\n")

        # mixed: 70% get, 30% set
        if i % 10 < 7:
            mixed_cmds.append(f"GET key_{i % (num_ops // 2)}\n")
        else:
            mixed_cmds.append(f"SET key_new_{i} value_{i}\n")

    # bench SET
    start = time.time()
    proc = subprocess.Popen(['./bin/server'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    cmds = ''.join(set_cmds) + 'QUIT\n'
    proc.communicate(input=cmds.encode())
    set_time = time.time() - start

    # bench GET (populate + get, then estimate get time)
    start = time.time()
    proc = subprocess.Popen(['./bin/server'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    cmds = ''.join(set_cmds) + ''.join(get_cmds) + 'QUIT\n'
    proc.communicate(input=cmds.encode())
    total_time = time.time() - start
    get_time = max(0.001, total_time - set_time)  # avoid zero/negative

    # bench MIXED
    start = time.time()
    proc = subprocess.Popen(['./bin/server'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    cmds = ''.join(set_cmds[:num_ops//2]) + ''.join(mixed_cmds) + 'QUIT\n'
    proc.communicate(input=cmds.encode())
    mixed_time = time.time() - start

    results['set_ops_per_sec'] = num_ops / set_time if set_time > 0 else 0
    results['get_ops_per_sec'] = num_ops / get_time if get_time > 0 else 0
    results['mixed_ops_per_sec'] = num_ops / mixed_time if mixed_time > 0 else 0
    results['set_time'] = set_time
    results['get_time'] = get_time
    results['mixed_time'] = mixed_time

    return results

# bench redis
def bench_redis(num_ops=1000):
    results = {}
    r = redis.Redis(host='localhost', port=6379, decode_responses=True)

    # flush before starting
    r.flushall()

    # bench SET
    start = time.time()
    for i in range(num_ops):
        r.set(f'key_{i}', f'value_{i}')
    set_time = time.time() - start

    # bench GET
    start = time.time()
    for i in range(num_ops):
        r.get(f'key_{i}')
    get_time = time.time() - start

    # bench MIXED (70% get, 30% set)
    start = time.time()
    for i in range(num_ops):
        if i % 10 < 7:
            r.get(f'key_{i % (num_ops // 2)}')
        else:
            r.set(f'key_new_{i}', f'value_{i}')
    mixed_time = time.time() - start

    results['set_ops_per_sec'] = num_ops / set_time
    results['get_ops_per_sec'] = num_ops / get_time
    results['mixed_ops_per_sec'] = num_ops / mixed_time
    results['set_time'] = set_time
    results['get_time'] = get_time
    results['mixed_time'] = mixed_time

    return results

if __name__ == '__main__':
    import sys

    num_ops = 1000
    if len(sys.argv) > 1:
        num_ops = int(sys.argv[1])

    print(f"running benchmarks with {num_ops} operations...\n")

    # start redis if not running
    subprocess.run(['redis-server', '--daemonize', 'yes', '--port', '6379'],
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(1)  # wait for redis to start

    print("benchmarking learndb...")
    learn_results = bench_learndb(num_ops)

    print("benchmarking redis...")
    redis_results = bench_redis(num_ops)

    # save results
    results = {
        'num_ops': num_ops,
        'learndb': learn_results,
        'redis': redis_results
    }

    with open('benchmark_results.json', 'w') as f:
        json.dump(results, f, indent=2)

    print("\nresults saved to benchmark_results.json")
    print("\nlearndb:")
    print(f"  set:   {learn_results['set_ops_per_sec']:.1f} ops/sec")
    print(f"  get:   {learn_results['get_ops_per_sec']:.1f} ops/sec")
    print(f"  mixed: {learn_results['mixed_ops_per_sec']:.1f} ops/sec")

    print("\nredis:")
    print(f"  set:   {redis_results['set_ops_per_sec']:.1f} ops/sec")
    print(f"  get:   {redis_results['get_ops_per_sec']:.1f} ops/sec")
    print(f"  mixed: {redis_results['mixed_ops_per_sec']:.1f} ops/sec")

    # shutdown redis
    subprocess.run(['redis-cli', 'shutdown'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
