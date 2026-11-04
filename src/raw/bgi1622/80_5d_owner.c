// BGI 1.622, immutable IDAPython export: 80:5D globals and their users.
//==== 489D90
int __cdecl sub_489D90(_DWORD *a1)
{
  sub_445480(a1);
  sub_48DC60();
  return 0;
}


//==== 48D860
int __cdecl sub_48D860(int a1)
{
  int v1; // esi
  int v2; // eax
  int v3; // eax
  int v4; // eax
  unsigned int i; // edi
  int v6; // ecx
  int (__cdecl *v7)(int); // eax
  _DWORD v9[6]; // [esp+0h] [ebp-148h] BYREF
  int v10; // [esp+18h] [ebp-130h]
  int v11; // [esp+1Ch] [ebp-12Ch]
  int v12; // [esp+20h] [ebp-128h]
  int v13; // [esp+24h] [ebp-124h]
  int v14; // [esp+28h] [ebp-120h]
  int v15; // [esp+2Ch] [ebp-11Ch]
  int v16; // [esp+30h] [ebp-118h]
  char Buffer[256]; // [esp+34h] [ebp-114h] BYREF
  _DWORD *v18; // [esp+138h] [ebp-10h]
  int v19; // [esp+144h] [ebp-4h]

  v18 = v9;
  v14 = a1;
  v16 = 0;
  v10 = 0;
  v1 = sub_4468E0();
  v15 = v1;
  v19 = 0;
  while ( v1 )
  {
    v13 = 1;
    v11 = 0;
    if ( dword_5668AC && v1 != dword_5668B0 )
      goto LABEL_28;
    v12 = 1;
    v2 = sub_445370();
    if ( v2 < 0 )
    {
      if ( !sub_4458D0() )
        v11 = v1;
      v12 = 0;
      goto LABEL_28;
    }
    if ( (v2 & 1) != 0 )
    {
      v3 = sub_4455C0();
      if ( !v3 )
        goto LABEL_13;
      if ( v3 == -1 )
      {
        v16 = 1;
LABEL_13:
        v12 = 0;
        goto LABEL_28;
      }
    }
    if ( !v16 )
    {
      v4 = 0;
      for ( i = 0; ; ++i )
      {
        v9[5] = i;
        if ( v4 || i >= 0x100000 )
          break;
        v6 = (unsigned __int8)sub_4453E0();
        v7 = funcs_48D98E[v6];
        if ( !v7 )
        {
          sprintf(Buffer, &byte_4EBF60, v6);
          sub_464C50(v1);
        }
        v4 = v7(v1);
      }
      switch ( v4 )
      {
        case 2:
          v13 = 0;
          goto LABEL_29;
        case 3:
          v1 = sub_444F60(dword_5668A8);
          v15 = v1;
          v13 = 0;
          goto LABEL_29;
        case 4:
          sub_445390(0x80000000);
          v13 = 0;
          goto LABEL_29;
        case 5:
          v10 = 1;
          goto LABEL_27;
        case 6:
          v16 = 1;
          goto LABEL_27;
        default:
LABEL_27:
          if ( v13 )
            break;
          goto LABEL_29;
      }
    }
LABEL_28:
    v1 = sub_4468E0();
    v15 = v1;
LABEL_29:
    if ( v11 )
      sub_444EF0(v11, v14);
  }
  v19 = -1;
  if ( v16 )
    return 1;
  else
    return v10 != 0 ? 2 : 0;
}


//==== 48DC60
int __usercall sub_48DC60@<eax>(int result@<eax>)
{
  dword_5668B0 = result;
  dword_5668AC = result != 0;
  return result;
}


