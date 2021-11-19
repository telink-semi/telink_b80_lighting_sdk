function show_info{
    param(
        [string]$info
    )
    Write-Host $info
}

function read_options {
    param (
        [string] $cproject_path
    )
    if(Test-path $cproject_path){
        $data = Get-Content $cproject_path
        # show_info $data
        [System.Collections.ArrayList]$stack_temp = @()
        [System.Collections.ArrayList]$position_cconfiguration = @()
        [System.Collections.ArrayList]$position_temp = @()
        for($i = 0; $i -lt $data.Count; $i++){
            $results = [regex]::Matches($data[$i], "<[a-zA-Z0-9]{1,}")
            if($results.Count -gt 0){
                for($j = 0; $j -lt $results.Count; $j++){
                    if($results[$j].Value -eq '<cconfiguration'){
                        $position_temp += $i
                        $stack_temp = @()
                        $stack_temp += $results[$j].Value
                        if($j -ne $results.Count){
                            for($k = $j+1; $k -lt $results.Count; $k++){
                                $stack_temp += $results[$k].Value
                            }
                        }
                        break
                    }
                    else{
                        $stack_temp += $results[$j].Value
                    }
                }
            }
            $results = [regex]::Matches($data[$i], "<?/[a-zA-Z]{0,}>")
            if($results.Count -gt 0){
                # show_info $results[0].Value
                for($j = 0; $j -lt $results.Count; $j++){
                    if(($stack_temp.Count -eq 1) -and ($position_temp.Count -eq 1)){
                        $position_temp += $i
                        $name = '' 
                        for($k = $position_temp[0]; $k -lt $position_temp[1]; $k++){
                            if($data[$k] -match '<storageModule'){
                                $result_temp = [regex]::Matches($data[$k], 'name="[0-9a-zA-Z_]{1,}"')
                                if($result_temp.Count -gt 0){
                                    $name = $result_temp[0].Value.Substring(6, $result_temp[0].Value.Length-7)
                                    # show_info $name
                                    break
                                }
                            }                        
                        }
                        $position_temp += $name
                        $position_cconfiguration += (,$position_temp)
                        $position_temp = @()
                        break
                    }
                    elseif ($stack_temp.Count -gt 0){
                        # show_info $position_temp
                        # show_info $stack_temp
                        $stack_temp.RemoveAt($stack_temp.Count-1)
                    }
                }
            }

        }
        return $position_cconfiguration
    }
    else{
        show_info "File '.cproject' does not exist."
        return @()
    }
}

# function remove_option {
#     param (
#         [string] $cproject_path,
#         [string] $option
#     )
#     $position_cconfiguration = read_options($cproject_path)
#     if($position_cconfiguration.Count -gt 0){
#         $begin = 0
#         $end = $begin
#         foreach ($cconfiguration in $position_cconfiguration){
#             # show_info $cconfiguration
#             if($cconfiguration[2] -eq $option){
#                 $begin = $cconfiguration[0]
#                 $end = $cconfiguration[1]
#                 show_info ("Remove complie option: " + $option)
#                 break
#             }
#         } 
#         $data_temp = @()
#         $data = Get-Content $cproject_path
#         if ($begin -ne $end){
#             for($i = 0; $i -lt $data.Count; $i++){
#                 if(($i -lt $begin) -or ($i -gt $end)){
#                     # show_info $data[$i]
#                     $data_temp += $data[$i]
#                 }
#             }
#         }
#         # show_info $data_temp
#         if($data_temp.Count -gt 0){
#             Set-Content $cproject_path $data_temp
#         }
#     } 
# }

function remove_options {
    param (
        [string] $cproject_path,
        [string] $value
    )
    [System.Collections.ArrayList]$scale_save = @()
    [System.Collections.ArrayList]$position_temp = @()
    $position_cconfiguration = read_options($cproject_path)
    $values = $value.Split(',')
    for($i = 0; $i -lt $values.Count; $i++){
        foreach ($cconfiguration in $position_cconfiguration){
            if($values[$i] -eq $cconfiguration[2]){
                $position_temp = @()
                $position_temp += $cconfiguration[0]
                $position_temp += $cconfiguration[1]
                $scale_save += (,$position_temp)
                show_info ("Remove complie option: " + $values[$i])
            }
        }
    }

    $data = Get-Content $cproject_path
    $data_temp = @()

    for($i = 0; $i -lt $data.Count; $i++)
    {
        $save_flag = $true
        foreach($scale in $scale_save){
            if($i -ge $scale[0] -and $i -le $scale[1]){
                $save_flag = $false
                break
            }
        }
        if($save_flag){
            $data_temp += $data[$i]
        }
    }

    if($data_temp.Count -gt 0){
        Remove-Item -Path $cproject_path -Recurse -Force
        Set-Content $cproject_path $data_temp
    }
}

function read_config{
    param (
        [string]$cfg_path,
        [string]$section,
        [string]$key
    )
    if(Test-path $cfg_path){
        $data = Get-Content $cfg_path
        $flag = $false
        $section_content = ''
        for($i = 0; $i -lt $data.count; $i++){
            if($data[$i] -match "\[$section\]"){
                $flag = $true
                continue
            }
            elseif($data[$i] -match "\[[a-zA-Z0-9]{1,}\]"){
                $flag = $false
            }
            if($flag){
                $section_content += $data[$i] + '::'
            }
        }
        $lines = $section_content.Split('::')
        $value = ''
        for($i = 0 ; $i -lt $lines.Count; $i++){
            $key_value = $lines[$i].Split('=')
            if($key_value[0].Equals($key)){
                $value = $key_value[1]
                break
            }
        }
        # show_info $value
        return $value
    }
    else{
        show_info "The configuration file does not exist."
        return ''
    }
}

function remove_files{
    param (
        [string]$project_path,
        [string]$value
    )
    # show_info $value
    $rm_path = $project_path
    $values = $value.Split(',')
    for($i = 0; $i -lt $values.Count; $i++){
        $rm_temp = $values[$i]
        # show_info $rm_temp
        if($rm_temp.EndsWith('\')){
            $rm_path = $project_path + $rm_temp
            # show_info $rm_path
        }
        else{
            $rm_file_dir = $rm_path + $rm_temp
            # show_info $rm_file_dir
            if(Test-Path $rm_file_dir){
                Remove-Item -Path $rm_file_dir -Recurse -Force
                show_info ('Remove ' + $rm_file_dir)
            }
        }
    }
}

function remove_headers{
    param (
        [string]$path,
        [string]$value
    )
    if(Test-Path $path){
        $values = $value.Split(',')
        $data = Get-Content $path
        $data_temp = @()
        for($i = 0; $i -lt $data.Count; $i++)
        {
            $flag = $true
            for($j = 0; $j -lt $values.Count; $j++){
                if($values[$j].EndsWith('.h')){
                    if($data[$i] -match $values[$j]){
                        $flag = $false
                        break
                    }
                }
            }
            if($flag){
                $data_temp += $data[$i]
            }
        }
        Set-Content $path $data_temp
    }
}

function show_help {
    show_info "usage:       [command] [para1][...]"
    show_info "command:     rm_options, rm_files, rd_options, rd_config, help"
    show_info "rm_options:  para[1] project path,               para[2] configuration file path,"
    show_info "             para[3] configuration section,      para[4] path of file .cproject."
    show_info "rm_files:    para[1] project path,               para[2] configuration file path,"
    show_info "             para[3] configuration section,      argv[4] file to remove include header"
    show_info "rd_options:  para[1] project path,               para[2] path of file .cproject,"
    show_info "             para[3] path to save read options."
    show_info "rd_config:   para[1] configuration file path,    para[2] configuration section,"
    show_info "             para[3] configuration key,          para[4] path to save read configures."
    show_info "help:        None."
}

function main{
param(
    [string]$argv0,
    [string]$argv1,
    [string]$argv2,
    [string]$argv3,
    [string]$argv4
)
$cmd = $argv0
switch($cmd){
    rm_options{
        if([string]::IsNullOrEmpty($argv4)){
            show_info "Incomplete parameters."
        }
        else{
            # show_info $argv0
            # show_info $argv1
            # show_info $argv2
            # show_info $argv3
            # show_info $argv4
            $project_path = $argv1
            if($project_path.EndsWith('\') -eq $false){
                $project_path += '\'
            }
            $config_path = $argv2
            $config_section = $argv3
            $cproject_path = $project_path + $argv4
            show_info $cproject_path
            $value = read_config $config_path $config_section "rm_options"
            if($value.Length -gt 0){
            #     $values = $value.Split(',')
                # for($i = 0; $i -lt $values.Count; $i++){
                #     # show_info $values[$i]
                #     remove_option $cproject_path $values[$i]
                # }
                remove_options $cproject_path $value
            }    
        }
    }
    rm_files{
        if([string]::IsNullOrEmpty($argv4)){
            # show_info $argv0
            # show_info $argv1
            # show_info $argv2
            # show_info $argv3
            if([string]::IsNullOrEmpty($argv3))
            {
                show_info "Incomplete parameters."
            }
            else{
                $project_path = $argv1    
                if($project_path.EndsWith('\') -eq $false){
                    $project_path += '\'
                }
                $config_path = $argv2
                $config_section = $argv3
                $value = read_config $config_path $config_section 'rm_files'
                # show_info $value
                if($value.Length -gt 0){
                    remove_files $project_path $value
                }
            }
        }
        else {
            $project_path = $argv1
            if($project_path.EndsWith('\') -eq $false){
                $project_path += '\'
            }
            $config_path = $argv2
            $config_section = $argv3
            $header_file_path = $project_path + $argv4
            $value = read_config $config_path $config_section 'rm_files'
            # show_info $value
            # show_info $header_file_path
            if($value.Length -gt 0){
                remove_headers $header_file_path $value
            }
        }
    }
    rd_options{
        if([string]::IsNullOrEmpty($argv3)){
            show_info "Incomplete parameters."
        }
        else{
            # show_info $argv0
            # show_info $argv1
            # show_info $argv2
            # show_info $argv3
            $project_path = $argv1
            if($project_path.EndsWith('\') -eq $false){
                $project_path += '\'
            }
            $cproject_path = $project_path + $argv2
            $path_save = $argv3
            # show_info $cproject_path
            # show_info $path_save
            $position_cconfiguration = read_options($cproject_path)
            if($position_cconfiguration.Count -gt 0){
                $data_temp = @()
                for($i = 0; $i -lt $position_cconfiguration.Count; $i++){
                    # show_info $position_cconfiguration[$i][2]
                    $data_temp += $position_cconfiguration[$i][2]
                }
                Set-Content $path_save $data_temp
            }
        }
    }
    rd_config{
        if([string]::IsNullOrEmpty($argv4)){
            show_info "Incomplete parameters."
        }
        else{
            $config_path = $argv1
            $config_section = $argv2
            $config_key = $argv3
            $path_save = $argv4
            $value = read_config $config_path $config_section $config_key
            # show_info $value
            if($value.Length -gt 0){
                $values = $value.Split(',')
                $data_temp = @()
                for($i = 0; $i -lt $values.count; $i++){
                    $data_temp += $values[$i]
                }
                Set-Content $path_save $data_temp
            }
        }
    }
    help{
        show_help
    }
    default{
        show_info "[command] error!"
        show_help
    }
}
}

main $args[0] $args[1] $args[2] $args[3] $args[4]
# read_config .\test_cfg.ini hello2 hello2
# remove_option .\.cproject B91_UART_Demo
# remove_files .\ 'test\,test1,test2.txt'
# remove_headers .\test\driver_b91.h 'dma.h,watchdog.h'