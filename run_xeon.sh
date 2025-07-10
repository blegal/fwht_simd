python3 ../build_all_N.py  --decoder dec3 --platform xeon
python3 ../build_all_N.py  --decoder dec4 --platform xeon
python3 ../build_all_GF.py --decoder dec3 --platform xeon
python3 ../build_all_GF.py --decoder dec4 --platform xeon
python3 ../build_all_R.py  --decoder dec3 --platform xeon --cores 1 --time 60
python3 ../build_all_R.py  --decoder dec4 --platform xeon --cores 1 --time 60



